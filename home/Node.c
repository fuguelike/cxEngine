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
        Node a = CX_TYPE_CAST(attacker,Node);
        NodeAddLife(this, -NodeGetPower(a));
    }else if(type == AttackTypeBullet){
        Bullet a = CX_TYPE_CAST(attacker,Bullet);
        NodeAddLife(this, -BulletGetPower(a));
    }
}

static void NodeAttackTimerArrive(cxAny pav)
{
    Node this = CX_TYPE_CAST(cxActionView(pav),Node);
    cxHash bindes = cxViewBindes(this);
    CX_HASH_FOREACH(bindes, ele, tmp){
        //获取目标
        Node target = cxHashElementKeyToAny(ele);
        //攻击前目标已死
        if(NodeCheckDie(target)){
            continue;
        }
        //是否可以攻击
        if(!CX_METHOD_GET(true, this->IsAttackTarget,this,target)){
            continue;
        }
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
        CX_LOGGER("attacker reset");
        cxActionReset(this->attackTimer);
    }else{
        this->attackTimer = cxViewAppendTimer(this, NodeGetAttackRate(this), CX_TIMER_FOREVER);
        cxActionSetGroup(this->attackTimer, "fight");
        CX_EVENT_APPEND(this->attackTimer->onArrive, NodeAttackTimerArrive);
    }
    cxActionResume(this->attackTimer);
}

cxBool NodeIsAttackTarget(cxAny pattacker,cxAny ptarget)
{
    Node attacker = CX_TYPE_CAST(pattacker,Node);
    Node target = CX_TYPE_CAST(ptarget,Node);
    //获取攻击者作战范围
    cxRange2f range = NodeGetRange(attacker);
    //计算作战距离
    cxFloat d = NodeDistance(attacker, target);
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
    this->searchIndex = 0;
    this->isDie = false;
    this->Index = cxVec2fv(-1, -1);
    NodeSetField(this, cxRange2fv(0, 6));
    CX_METHOD_SET(this->NodeAttacked, NodeAttacked);
    CX_METHOD_SET(this->IsAttackTarget, NodeIsAttackTarget);
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

//到达指定位置
static void NodeMoveArrive(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Node node = CX_TYPE_CAST(cxActionView(this),Node);
    cxHash bindes = cxViewBindes(node);
    CX_HASH_FOREACH(bindes, ele, tmp){
        Node target = cxHashElementKeyToAny(ele);
        //目标死了用不着移动了
        if(NodeCheckDie(target)){
            continue;
        }
        //回调处理返回,返回false表示无需攻击目标
        if(!CX_METHOD_GET(true, node->MoveExit, node, target)){
            cxViewUnBind(node, target);
            continue;
        }
        //朝向target
        NodeFaceTarget(node, target);
        //发动攻击 目标已经bind到node
        cxViewBind(node, target, cxNumberInt(NodeBindReasonAttack));
        NodeStartupAttackTimer(node);
    }
}

cxFloat NodeDistance(cxAny src,cxAny dst)
{
    Node snode = CX_TYPE_CAST(src,Node);
    Node dnode = CX_TYPE_CAST(dst,Node);
    cxVec2f sidx = NodeGetIndex(snode);
    cxVec2f didx = NodeGetIndex(dnode);
    cxFloat d = kmVec2DistanceBetween(&sidx, &didx) - (NodeGetBody(snode) + NodeGetBody(dnode));
    d = CX_MAX(d, 0);
    return d;
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
    //bind 目标 移动
    cxViewBind(pview, target, cxNumberInt(NodeBindReasonMove));
    Move move = MoveCreate(map, points);
    CX_EVENT_APPEND(CX_TYPE(cxAction, move)->onExit, NodeMoveArrive);
    cxViewAppendAction(this, move);
}

void NodeFaceTarget(cxAny pview,cxAny target)
{
    CX_ASSERT_THIS(pview, Node);
    cxFloat angle = cxVec2fRadiansBetween(cxViewPosition(target),cxViewPosition(this));
    NodeSetDirAngle(this, angle);
}

static void NodeProcessSearch(Node this)
{
    cxHash bindes = cxViewBindes(this);
    //如果同时攻击数量到达
    if(cxHashLength(bindes) >= NodeGetAttackNum(this)){
        return;
    }
    //如果未定义搜索
    if(this->orders.number == 0){
        return;
    }
    if(this->searchIndex >= this->orders.number){
        this->searchIndex = 0;
    }
    cxRange2f field = NodeGetField(this);
    Node target = NULL;
    //获取当前搜索类型
    NodeCombined type = this->orders.types[this->searchIndex++];
    //先搜索视野范围内可以到达的目标
    if(target == NULL){
        target = MapNearestQuery(this, type, field, true);
    }
    //搜索视野范围内最近的目标
    if(target == NULL){
        target = MapNearestQuery(this, type, field, false);
    }
    //搜索最大范围内的目标
    if(target == NULL){
        target = MapNearestQuery(this, type, MAX_RANGE, false);
    }
    if(target == NULL){
        return;
    }
    //立即面向目标
    NodeFaceTarget(this,target);
    cxBool isAttack = false;
    //target被this发现,返回false表示target不能被攻击
    isAttack = CX_METHOD_GET(true, target->NodeFinded,target,this);
    if(!isAttack){
        return;
    }
    //返回bind的目标
    target = CX_METHOD_GET(NULL,this->FindTarget,this,target);
    if(target == NULL){
        return;
    }
    //bind目标
    cxViewBind(this, target, cxNumberInt(NodeBindReasonAttack));
    //目标已经bind到this,启动攻击定器
    NodeStartupAttackTimer(this);
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
    NodeProcessSearch(this);
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
    if(life.min <= 0 && !this->isDie){
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
}













