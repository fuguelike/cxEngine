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

static void NodeOnTransform(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
    cxVec2f pos = cxViewPosition(this);
    this->Index = MapPosToFloat(map, pos);
    cxSpatialReindexView(map->items, this);
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

void NodeAddLife(cxAny pview,cxInt life)
{
    CX_ASSERT_THIS(pview, Node);
    this->Life.min += life;
    CX_EVENT_FIRE(this, onLife);
}

void NodeAttacked(cxAny pview,cxAny attacker,AttackType type)
{
    CX_ASSERT_THIS(pview, Node);
    //如果是直接攻击
    if(type == AttackTypeDirect){
        CX_ASSERT_VALUE(attacker, Node, a);
        NodeAddLife(this, -NodeGetPower(a));
    }else if(type == AttackTypeBullet){
        CX_ASSERT_VALUE(attacker, Bullet, a);
        NodeAddLife(this, -BulletGetPower(a));
    }
}

static void NodeAttackTimerArrive(cxAny pav)
{
    CX_ASSERT_THIS(cxActionView(pav), Node);
    cxHash bindes = cxViewBindes(this);
    CX_HASH_FOREACH(bindes, ele, tmp){
        //获取目标
        Node target = cxHashElementKeyToAny(ele);
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
        CX_METHOD_RUN(this->AttackTarget, this, target, ele->any);
        //攻击后目标死亡
        if(NodeCheckDie(target)){
            continue;
        }
    }
    //如果没有目标停止攻击定时器
    if(cxHashLength(bindes) == 0){
        cxActionPause(pav);
    }
}

void NodeStartupAttackTimer(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    if(this->attackTimer != NULL){
        cxActionReset(this->attackTimer);
    }else{
        this->attackTimer = cxViewAppendTimer(this, NodeGetAttackRate(this), CX_TIMER_FOREVER);
        cxActionSetGroup(this->attackTimer, "fight");
        CX_EVENT_APPEND(this->attackTimer->onArrive, NodeAttackTimerArrive);
    }
    cxActionResume(this->attackTimer);
}

cxBool NodeIsArriveRange(cxAny pattacker,cxAny ptarget)
{
    CX_ASSERT_VALUE(pattacker, Node, attacker);
    CX_ASSERT_VALUE(ptarget, Node, target);
    cxVec2f sidx = NodeGetIndex(attacker);
    cxVec2f didx = NodeGetIndex(target);
    cxFloat d = kmVec2DistanceBetween(&sidx, &didx);
    //获取攻击者作战范围
    cxRange2f range = NodeGetRange(attacker);
    //检测是否在作战范围内
    return d >= range.min && d <= range.max;
}

CX_OBJECT_TYPE(Node, cxSprite)
{
    
}
CX_OBJECT_INIT(Node, cxSprite)
{
    CX_EVENT_APPEND(CX_TYPE(cxView, this)->onTransform, NodeOnTransform);
    NodeSetAttackNum(this, 1);
    NodeSetIndex(this, cxVec2fv(-1, -1));
    NodeSetField(this, cxRange2fv(0, 6));
    NodeSetSearchRate(this, 0.5f);
    NodeSetAttackRate(this, 0.5f);
    CX_METHOD_SET(this->NodeAttacked, NodeAttacked);
    //Test
    CX_EVENT_APPEND(this->onLife, LifeChange);
}
CX_OBJECT_FREE(Node, cxSprite)
{
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
    CX_METHOD_RUN(this->NodeDirection,this);
    //Test
    cxViewSetDegrees(this->array, dirIndex * 45.0f);
}

cxFloat NodeDistance(cxAny src,cxAny dst)
{
    CX_ASSERT_VALUE(src, Node, snode);
    CX_ASSERT_VALUE(dst, Node, dnode);
    cxVec2f sidx = NodeGetIndex(snode);
    cxVec2f didx = NodeGetIndex(dnode);
    return kmVec2DistanceBetween(&sidx, &didx);
}

//到达指定位置
static void NodeMoveToTargetArrive(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    CX_ASSERT_VALUE(cxActionView(this), Node, node);
    cxHash bindes = cxViewBindes(node);
    CX_HASH_FOREACH(bindes, ele, tmp){
        Node target = cxHashElementKeyToAny(ele);
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

void NodeMovingToTarget(cxAny pview,cxAny target, cxAnyArray points)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
    //Test show point position
    cxList subviews = cxViewSubViews(map->aLayer);
    CX_LIST_FOREACH(subviews, ele){
        cxView tmp = ele->any;
        if(tmp->tag == 1001){
            cxViewRemove(tmp);
        }
    }
    CX_ASTAR_POINTS_FOREACH(points, idx){
        cxVec2f p = cxVec2fv(idx->x + 0.5f, idx->y + 0.5f);
        cxVec2f pos = MapIndexToPos(map, p);
        cxSprite sp = cxSpriteCreateWithURL("bullet.json?shell.png");
        cxViewSetColor(sp, cxWHITE);
        cxViewSetPos(sp, pos);
        cxViewSetSize(sp, cxSize2fv(8, 8));
        cxViewSetTag(sp, 1001);
        cxViewAppend(map->aLayer, sp);
    }
    //使用点集合移动this
    Move move = MoveCreate(this, points);
    CX_EVENT_APPEND(CX_TYPE(cxAction, move)->onExit, NodeMoveToTargetArrive);
    cxViewAppendAction(this, move);
}

void NodeFaceTarget(cxAny pview,cxAny target)
{
    CX_ASSERT_THIS(pview, Node);
    cxFloat angle = cxVec2fRadiansBetween(cxViewPosition(target),cxViewPosition(this));
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

void NodeAttackTarget(cxAny attacker,cxAny target,AttackType type)
{
    CX_ASSERT_THIS(target, Node);
    CX_METHOD_RUN(this->NodeAttacked,this,attacker,type);
    //攻击后死亡了
    NodeCheckDie(target);
}

//搜索附近的
static void NodeSearchArrive(cxAny pav)
{
    cxAny pview = cxActionView(pav);
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
    //路径规则结果
    PathRuleResult pret = PathRuleResultEmpty();
    //搜索规则结果
    FindRuleResult fret = FindRuleResultEmpty();
    const NodeCombined *type = NodeGetSearchType(this);
    if(type == NULL){
        goto attack;
    }
    cxHash bindes = cxViewBindes(this);
    //如果同时攻击数量到达
    if(cxHashLength(bindes) >= NodeGetAttackNum(this)){
        goto attack;
    }
    CX_ASSERT(this->FindRule, "no implement search rule");
    //启动搜索规则
     fret = CX_METHOD_GET(fret, this->FindRule, this, type);
    //在搜索规则下没有发现目标
    if(fret.target == NULL){
        goto attack;
    }
    //已经到达攻击范围无需寻路
    if(NodeIsArriveRange(this, fret.target)){
        pret = PathRuleResultMake(fret.target, NodeBindReasonAttack);
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
    this->searchTimer = cxViewAppendTimer(this, NodeGetSearchRate(this), CX_TIMER_FOREVER);
    cxActionSetGroup(this->searchTimer, "fight");
    CX_EVENT_APPEND(this->searchTimer->onArrive, NodeSearchArrive);
}

cxBool NodeIsStatic(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->isStatic;
}

cxBool NodeCheckDie(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    //当前生命值太小表示死掉了
    cxRange2i life = this->Life;
    cxBool die = life.min <= 0;
    if(die && !this->isDie){
        CX_LOGGER("Node (type=%s) die At(%f %f)",CX_TYPE(cxObject, this)->cxType,this->Index.x,this->Index.y);
        //防止死的次数太多
        this->isDie = true;
        this->Life.min = 0;
        CX_EVENT_FIRE(this, onLife);
        CX_EVENT_FIRE(this, onDie);
        cxViewUnBindAll(pview);
        MapRemoveNode(pview);
    }
    return die;
}

void NodeInitIndex(cxAny pview,cxVec2i idx)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeGetMap(this);
    this->initIdx = idx;
    //为了对齐格子加上node大小
    cxVec2f fidx = cxVec2fv(idx.x + this->Size.w/2.0f, idx.y + this->Size.h/2.0f);
    
    cxVec2f npos = MapIndexToPos(map, fidx);
    cxViewSetPos(this, npos);
    //获取精确的格子坐标
    this->Index = MapPosToFloat(map, npos);
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
    this->isStatic = isStatic;
    NodeInitIndex(this, idx);
    MapFillNode(map, idx, this);
    //show test array
    cxSize2f ns = cxViewSize(this);
    cxFloat w = CX_MIN(ns.w, ns.h);
    this->array = cxSpriteCreateWithURL("bullet.json?array.png");
    cxViewSetColor(this->array, cxWHITE);
    cxViewSetAnchor(this->array, cxVec2fv(-0.5f, 0.0f));
    cxViewSetSize(this->array, cxSize2fv(w/2, w/3));
    cxViewAppend(this, this->array);
    
    this->lifeTTF = cxLabelTTFCreate(UTF8("100"), NULL, 20);
    cxViewSetColor(this->lifeTTF, cxWHITE);
    cxViewSetAutoResizeMask(this->lifeTTF, cxViewAutoOutside| cxViewAutoResizeTop);
    cxViewAppend(this, this->lifeTTF);
}













