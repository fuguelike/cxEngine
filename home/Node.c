//
//  Node.c
//  Home
//
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <algorithm/cxTile.h>
#include <views/cxScroll.h>
#include <actions/cxMove.h>
#include "Bullet.h"
#include "Node.h"
#include "Move.h"
#include "Map.h"

static void NodeOnDirty(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
    cxVec2f pos = cxViewGetPosition(this);
    this->Index = MapPosToFloat(map, pos);
    cxSpatialReindexView(map->items, this);
    cxVec2i idx = cxVec2fTo2i(this->Index);
    CX_RETURN(cxVec2iEqu(this->prevIdx, idx));
    this->prevIdx = idx;
    CX_EVENT_FIRE(this, onIndex);
}

//设置优先攻击顺序
void NodeSetSearchOrder(cxAny pview,NodeType type,NodeSubType subType)
{
    CX_ASSERT_THIS(pview, Node);
    NodeCombined *ptype = &this->orders.types[this->orders.number++];
    ptype->mainType = type;
    ptype->subType = subType;
}
//清空搜索
void NodeSearchOrderClear(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    this->orders.number = 0;
}

static void LifeChange(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    cxLabelTTFSetText(this->lifeTTF, UTF8("%d/%d",this->Life.min,this->Life.max));
}

void NodeAttacked(cxAny pview,cxAny attacker,AttackType type)
{
    CX_ASSERT_THIS(pview, Node);
    if(type == AttackTypeNode){
        CX_ASSERT_VALUE(attacker, Node, a);
        NodeAddLife(this, -NodeGetPower(a));
    }else if(type == AttackTypeBullet){
        CX_ASSERT_VALUE(attacker, Bullet, a);
        NodeAddLife(this, -BulletGetPower(a));
    }
}

void NodeAttackTarget(cxAny attacker,cxAny target,AttackType type)
{
    CX_ASSERT_THIS(target, Node);
    CX_METHOD_RUN(this->NodeAttacked,this,attacker,type);
    //攻击后死亡了？
    NodeCheckDie(target);
}

//执行攻击结束
static void NodeHasAttackActionExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_ASSERT_VALUE(cxActionGetView(this), Node, node);
    Node target = cxViewBindesFirst(node);
    CX_RETURN(target == NULL);
    NodeAttackTarget(node, target, AttackTypeNode);
    CX_METHOD_RUN(node->AttackOnce, node, target);
}
//不执行攻击结束
static void NodeNotAttackActionExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_ASSERT_VALUE(cxActionGetView(this), Node, node);
    Node target = cxViewBindesFirst(node);
    CX_RETURN(target == NULL);
    CX_METHOD_RUN(node->AttackOnce, node, target);
}

static void NodeAttackBulletExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_ASSERT_VALUE(cxActionGetView(this), Bullet, bullet);
    //如果有bind的目标就攻击他
    cxAny target = NULL;
    BulletGetNode(bullet, NULL, &target);
    //如果目标还存在进行攻击
    if(target != NULL){
        NodeAttackTarget(bullet, target, AttackTypeBullet);
    }
    //移除bullet
    MapRemoveBullet(bullet);
}

static void BulletOnUpdate(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_ASSERT_VALUE(cxActionGetView(this), Bullet, bullet);
    CX_METHOD_RUN(bullet->onUpdate,bullet, this);
}

static void NodeInitBullet(Node this,Node target,cxAny bullet)
{
    CX_ASSERT_TYPE(bullet, Bullet);
    BulletBind(bullet, this, target);
    
    cxAny baction = BulletCreateEngine(bullet);
    CX_ASSERT_TYPE(baction, cxAction);
    cxActionSetGroup(baction, FIGHT_ACTION_GROUP);
    
    cxViewAppendAction(bullet, baction);
    MapAppendBullet(bullet);
    
    //带弹药结束 action.view = bullet
    CX_ADD(cxAction, baction, onExit, NodeAttackBulletExit);
    //bullet飞行过程ing
    CX_ADD(cxAction, baction, onUpdate, BulletOnUpdate);
}

static void NodeInitAction(Node this,Node target,cxAny action,cxBool hasBullet)
{
    //带动画结束 action.view = node
    cxViewAppendAction(this, action);
    cxActionSetGroup(action, FIGHT_ACTION_GROUP);
    //如果使用bullet攻击则不执行攻击动作
    if(hasBullet){
        CX_ADD(cxAction, action, onExit, NodeNotAttackActionExit);
    }else{
        CX_ADD(cxAction, action, onExit, NodeHasAttackActionExit);
    }
}

static void NodeAttackTimerArrive(cxAny pav)
{
    CX_ASSERT_THIS(cxActionGetView(pav), Node);
    cxHash bindes = cxViewGetBindes(this);
    CX_HASH_FOREACH(bindes, ele, tmp){
        //获取目标
        CX_ASSERT_VALUE(cxHashKeyToAny(ele), Node, target);
        //攻击前目标已死
        if(NodeCheckDie(target)){
            continue;
        }
        //如果超出攻击范围重新搜索
        if(!NodeIsArriveRange(this, target)){
            cxViewUnBind(this, target);
            continue;
        }
        //朝向target攻击
        NodeFaceTarget(this, target);
        //设置为攻击状态
        NodeSetState(this, NodeStateAttack);
        //创建攻击动画
        ActionResult ret = CX_METHOD_GET(AAEmpty(), this->AttackAction, this, target);
        //初始化bullet准备发射
        if(ret.bullet != NULL){
            NodeInitBullet(this, target, ret.bullet);
        }
        //创建自身动作
        if(ret.action != NULL){
            NodeInitAction(this, target, ret.action, ret.bullet != NULL);
        }
        //攻击后目标死亡
        if(NodeCheckDie(target)){
            continue;
        }
    }
    //如果没有目标停止攻击定时器
    if(cxHashLength(bindes) == 0){
        NodeSetState(this, NodeStateStatic);
        cxActionPause(pav);
    }
}

void NodeStartupAttackTimer(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    if(this->attackTimer != NULL){
        cxActionReset(this->attackTimer);
    }else{
        this->attackTimer = cxViewAppendTimer(this, NodeGetAttackRate(this), CX_FOREVER);
        cxActionSetGroup(this->attackTimer, FIGHT_ACTION_GROUP);
        CX_ADD(cxTimer, this->attackTimer, onArrive, NodeAttackTimerArrive);
    }
    cxActionResume(this->attackTimer);
}

cxBool NodeIsArriveDistance(cxAny pattacker,cxAny ptarget,cxFloat dis)
{
    CX_ASSERT_VALUE(pattacker, Node, attacker);
    CX_ASSERT_VALUE(ptarget, Node, target);
    dis -= NodeGetBody(attacker) + NodeGetBody(target);
    dis = CX_MAX(dis, 0);
    //获取攻击者作战范围
    cxRange2f range = NodeGetRange(attacker);
    //检测是否在作战范围内
    return dis >= range.min && dis <= range.max;
}

cxBool NodeIsArriveRange(cxAny pattacker,cxAny ptarget)
{
    CX_ASSERT_VALUE(pattacker, Node, attacker);
    CX_ASSERT_VALUE(ptarget, Node, target);
    cxVec2f sidx = NodeGetIndex(attacker);
    cxVec2f didx = NodeGetIndex(target);
    cxFloat d = kmVec2DistanceBetween(&sidx, &didx);
    return NodeIsArriveDistance(attacker,target,d);
}

CX_OBJECT_TYPE(Node, cxSprite)
{
    
}
CX_OBJECT_INIT(Node, cxSprite)
{
    CX_ADD(cxView, this, onDirty, NodeOnDirty);
    NodeSetAttackNum(this, 1);
    NodeSetIndex(this, cxVec2fv(-1, -1));
    NodeSetField(this, cxRange2fv(0, 6));
    NodeSetSearchRate(this, 0.5f);
    NodeSetAttackRate(this, 0.3f);
    NodeSetBody(this, 0.5f);
    NodeSetState(this, NodeStateStatic);
    CX_SET(Node, this, NodeAttacked, NodeAttacked);
    //Test
    CX_ADD(Node, this, onLife, LifeChange);
}
CX_OBJECT_FREE(Node, cxSprite)
{
    CX_EVENT_RELEASE(this->onState);
    CX_EVENT_RELEASE(this->onIndex);
    CX_EVENT_RELEASE(this->onDie);
    CX_EVENT_RELEASE(this->onLife);
}
CX_OBJECT_TERM(Node, cxSprite)

void NodeSetDirAngle(cxAny pview,cxFloat angle)
{
    CX_ASSERT_THIS(pview, Node);
    cxInt dirIndex;
    this->DirAngle = AngleToIndex(angle, &dirIndex);
    if(dirIndex == this->DirIndex){
        return;
    }
    this->DirIndex = dirIndex;
    CX_EVENT_FIRE(this, onState);
    //Test
    cxViewSetDegrees(this->array, dirIndex * 45.0f);
}

//到达指定位置
static void NodeMoveToTargetArrive(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    CX_ASSERT_VALUE(cxActionGetView(this), Node, node);
    cxHash bindes = cxViewGetBindes(node);
    CX_HASH_FOREACH(bindes, ele, tmp){
        Node target = cxHashKeyToAny(ele);
        //目标死了用不着移动了
        if(NodeCheckDie(target)){
            continue;
        }
        //移动结束如果没有达到作战距离,解除bind继续搜索
        if(!NodeIsArriveRange(node, target)){
            cxViewUnBind(node, target);
            continue;
        }
        //朝向target
        NodeFaceTarget(node, target);
        //发动攻击 重新bind目标为攻击状态
        cxViewBind(node, target, cxNumberInt(NodeBindReasonAttack));
        NodeStartupAttackTimer(node);
    }
}

void NodeMoveToPosition(cxAny pview,cxAnyArray points)
{
    CX_ASSERT_THIS(pview, Node);
    Move move = MoveCreate(this, points);
    MoveSetType(move, MoveTypePosition);
    cxViewAppendAction(this, move);
}

void NodeMovingToTarget(cxAny pview,cxAny target, cxAnyArray points)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
//    Test show point position
    CX_VIEW_FOREACH_SUBVIEWS(map->aLayer, ele){
        cxView tmp = ele->any;
        if(cxViewGetTag(tmp) == 1001){
            cxViewRemove(tmp);
        }
    }
    CX_ASTAR_POINTS_FOREACH(points, idx){
        cxVec2f p = cxVec2fv(idx->x + 0.5f, idx->y + 0.5f);
        cxVec2f pos = MapIndexToPos(map, p);
        cxSprite sp = cxSpriteCreateWithURL("bullet.json?shell.png");
        cxViewSetColor(sp, cxWHITE);
        cxViewSetPosition(sp, pos);
        cxViewSetSize(sp, cxSize2fv(8, 8));
        cxViewSetTag(sp, 1001);
        cxViewAppend(map->aLayer, sp);
    }
    //
    if(cxAnyArrayLength(points) < 2){
        //发动攻击 重新bind目标为攻击状态
        cxViewBind(this, target, cxNumberInt(NodeBindReasonAttack));
        NodeStartupAttackTimer(this);
    }else{
        //使用点集合移动this
        Move move = MoveCreate(this, points);
        MoveSetType(move, MoveTypeFight);
        CX_ADD(cxAction, move, onExit, NodeMoveToTargetArrive);
        cxViewAppendAction(this, move);
    }
}

void NodeFaceTarget(cxAny pview,cxAny target)
{
    CX_ASSERT_THIS(pview, Node);
    cxFloat angle = cxVec2fRadiansBetween(cxViewGetPosition(target),cxViewGetPosition(this));
    if(angle != FLT_EPSILON){
        NodeSetDirAngle(this, angle);
    }
}

static const NodeCombined *NodeGetSearchType(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    if(this->orders.number == 0){
        return NULL;
    }
    if(this->searchIndex >= this->orders.number){
        this->searchIndex = 0;
    }
    return &this->orders.types[this->searchIndex++];
}

//搜索附近的
static void NodeSearchArrive(cxAny pav)
{
    cxAny pview = cxActionGetView(pav);
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
    //路径规则结果
    PathRuleResult pret = PREmpty();
    //搜索规则结果
    FindRuleResult fret = FREmpty();
    const NodeCombined *type = NodeGetSearchType(this);
    if(type == NULL){
        goto attack;
    }
    cxHash bindes = cxViewGetBindes(this);
    //如果同时攻击数量到达
    if(cxHashLength(bindes) >= NodeGetAttackNum(this)){
        goto attack;
    }
    //必须实现搜索接口
    CX_ASSERT(this->FindRule != NULL, "no implement search rule");
    //启动搜索规则
    fret = CX_METHOD_GET(fret, this->FindRule, this, type);
    //在搜索规则下没有发现目标
    if(fret.target == NULL){
        goto attack;
    }
    //已经到达攻击范围无需寻路
    if(NodeIsArriveRange(this, fret.target)){
        pret = PRMake(fret.target, NodeBindReasonAttack);
        goto attack;
    }
    //路径搜索bind的目标传入搜索结果
    pret = CX_METHOD_GET(pret,this->PathRule,this,&fret);
attack:
    //返回NULL表示未发现路径
    if(pret.target == NULL){
        return;
    }
    //立即面向目标
    NodeFaceTarget(this,pret.target);
    //为移动bind
    if(pret.bd == NodeBindReasonMove){
        cxViewBind(this, pret.target, cxNumberInt(pret.bd));
        NodeMovingToTarget(this, pret.target, MapSearchPoints(map));
        return;
    }
    //为攻击bind
    if(pret.bd == NodeBindReasonAttack){
        cxViewBind(this, pret.target, cxNumberInt(pret.bd));
        NodeStartupAttackTimer(this);
        return;
    }
}

void NodePauseSearch(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    CX_RETURN(this->searchTimer == NULL);
    cxActionPause(this->searchTimer);
}

void NodeResumeSearch(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    CX_RETURN(this->searchTimer == NULL);
    cxActionReset(this->searchTimer);
    cxActionResume(this->searchTimer);
}

void NodeSearchRun(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
    //战斗地图下才启动搜索
    if(map->mode != MapModeFight){
        return;
    }
    this->searchTimer = cxViewAppendTimer(this, NodeGetSearchRate(this), CX_FOREVER);
    cxActionSetGroup(this->searchTimer, FIGHT_ACTION_GROUP);
    CX_ADD(cxTimer, this->searchTimer, onArrive, NodeSearchArrive);
}

cxBool NodeHited(cxAny pview,cxVec2f index)
{
    CX_ASSERT_THIS(pview, Node);
    cxBox4f box = NodeGetBox(this);
    return cxBox4fContainPoint(box, index);
}

cxBool NodeCheckDie(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
    if(map->mode != MapModeFight){
        return false;
    }
    //当前生命值太小表示死掉了
    cxRange2i life = this->Life;
    cxBool die = life.min <= 0;
    if(die && !this->isDie){
        CX_LOGGER("Node (type=%s) die At(%f %f)",CX_TYPE(cxObject, this)->cxType,this->Index.x,this->Index.y);
        //防止死的次数太多
        this->isDie = true;
        this->Life.min = 0;
        //死亡状态
        NodeSetState(this, NodeStateDie);
        CX_EVENT_FIRE(this, onLife);
        CX_EVENT_FIRE(this, onDie);
        cxViewUnBindAll(pview);
        MapRemoveNode(pview);
    }
    return die;
}

cxVec2i NodeIndexToInitIndex(cxAny pview,cxVec2f idx)
{
    CX_ASSERT_THIS(pview, Node);
    cxSize2i size = NodeGetSize(this);
    cxVec2f oidx = cxRoundVec2f(idx);
    oidx.x -= ((cxFloat)size.w / 2.0f);
    oidx.y -= ((cxFloat)size.h / 2.0f);
    return cxVec2iv(oidx.x, oidx.y);
}

cxVec2i NodeGetInitIndexUseIndex(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    cxVec2f idx = NodeGetIndex(this);
    return NodeIndexToInitIndex(this, idx);
}

void NodeResetIndex(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    NodeUpdateIndex(this, this->InitIndex);
}

cxBool NodeUpdateInitIndex(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
    cxVec2i newInitIdx = NodeGetInitIndexUseIndex(this);
    if(!MapIsFillNode(map, newInitIdx, this)){
        return false;
    }
    NodeUpdateIndex(this, newInitIdx);
    return true;
}

void NodeUpdateIndex(cxAny pview,cxVec2i idx)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
    //为了对齐格子加上node大小
    cxVec2f fidx = cxVec2fv(idx.x + this->Size.w/2.0f, idx.y + this->Size.h/2.0f);
    cxVec2f npos = MapIndexToPos(map, fidx);
    cxViewSetPosition(this, npos);
    //获取精确的格子坐标
    this->Index = MapPosToFloat(map, npos);
    //如果位置没改变不更新
    if(cxVec2iEqu(this->InitIndex, idx)){
        return;
    }
    CX_METHOD_RUN(map->NodeMove,map,this,this->InitIndex,idx);
    NodeSetInitIndex(this, idx);
    this->prevIdx = idx;
    if(MapIsFillNode(map, idx, this)){
        MapDetachNode(this);
        MapFillNode(map, idx, this);
    }
}

void NodeSetSize(cxAny pview,cxSize2i size)
{
    CX_ASSERT_THIS(pview, Node);
    cxSize2f vsize = cxSize2fv(global.unitSize.w * size.w, global.unitSize.h * size.h);
    cxViewSetSize(this, vsize);
    this->Size = size;
}

void NodeInit(cxAny pview,cxAny map,cxVec2i idx,cxBool isStatic)
{
    CX_ASSERT_THIS(pview, Node);
    this->Map = map;
    NodeSetIsStatic(this, isStatic);
    NodeUpdateIndex(this, idx);
    //show test array
    cxSize2f ns = cxViewGetSize(this);
    cxFloat w = CX_MIN(ns.w, ns.h);
    this->array = cxSpriteCreateWithURL("bullet.json?array.png");
    cxViewSetColor(this->array, cxWHITE);
    cxViewSetAnchor(this->array, cxVec2fv(-0.5f, 0.0f));
    cxViewSetSize(this->array, cxSize2fv(w/2, w/3));
    cxViewAppend(this, this->array);
    
    this->lifeTTF = cxLabelTTFCreate(UTF8("100"), NULL, 20);
    cxViewSetColor(this->lifeTTF, cxWHITE);
    cxViewSetAutoMask(this->lifeTTF, cxViewAutoOutside| cxViewAutoResizeTop);
    cxViewAppend(this, this->lifeTTF);
}













