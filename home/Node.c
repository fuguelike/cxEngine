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
    Map map = this->map;
    cxVec2f pos = cxViewPosition(this);
    this->index = MapPosToFloat(map, pos);
    cxSpatialReindexView(map->items, this);
}

//设置优先攻击顺序
void NodeSetSearchOrder(cxAny pview,NodeType type,NodeSubType subType,cxRange2f range)
{
    CX_ASSERT_THIS(pview, Node);
    NodeCombined *ptype = &this->orders.types[this->orders.number++];
    ptype->mainType = type;
    ptype->subType = subType;
    ptype->range = range;
}
//清空搜索
void NodeSearchOrderClear(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    this->orders.number = 0;
}

void NodeAttacked(cxAny pview,cxAny attacker,AttackType type)
{
    CX_ASSERT_THIS(pview, Node);
    //如果是直接攻击
    if(type == AttackTypeDirect){
        Node a = CX_TYPE_CAST(Node, attacker);
        NodeAddLife(this, -a->power);
    }else if(type == AttackTypeBullet){
        Bullet a = CX_TYPE_CAST(Bullet, attacker);
        NodeAddLife(this, -a->power);
    }
}

static void NodeAttackTimerArrive(cxAny pav)
{
    Node this = CX_TYPE_CAST(Node, cxActionView(pav));
    cxHash bindes = cxViewBindes(this);
    CX_HASH_FOREACH(bindes, ele, tmp){
        //获取目标
        Node target = cxHashElementKeyToAny(ele);
        //攻击前已死
        if(NodeCheckDie(target)){
            continue;
        }
        //是否可以攻击
        cxBool ret = CX_METHOD_GET(true, this->IsAttackTarget,this,target);
        if(!ret){
            continue;
        }
        CX_METHOD_RUN(this->AttackTarget,this,target);
        //攻击后死亡
        if(NodeCheckDie(target)){
            continue;
        }
    }
    //如果没有目标停止攻击定时器
    if(cxHashLength(bindes) == 0){
        cxActionStop(pav);
    }
}

void NodeStartupAttackTimer(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    cxTimer timer = cxEngineCreateObject("fights.json?attackTimer");
    CX_ASSERT_TYPE(timer, cxTimer);
    cxActionSetTime(timer, NodeAttackRate(this));
    CX_EVENT_APPEND(timer->onArrive, NodeAttackTimerArrive);
    cxViewAppendAction(this, timer);
}

static cxBool NodeIsAttackTarget(cxAny psx,cxAny pdx)
{
    Node sx = CX_TYPE_CAST(Node, psx);
    Node dx = CX_TYPE_CAST(Node, pdx);
    cxVec2f sidx = NodeFloatIndex(sx);
    cxVec2f didx = NodeFloatIndex(dx);
    cxFloat d = kmVec2DistanceBetween(&sidx, &didx) - (sx->body + dx->body);
    d = CX_MAX(d, 0);
    cxRange2f range = NodeRange(sx);
    return d >= range.min && d <= range.max;
}

CX_OBJECT_TYPE(Node, cxSprite)
{
    
}
CX_OBJECT_INIT(Node, cxSprite)
{
    CX_EVENT_APPEND(CX_TYPE(cxView, this)->onTransform, NodeOnTransform);
    this->attackNum = 1;
    this->searchIndex = 0;
    this->dirIndex = 0;
    this->isDie = false;
    this->index = cxVec2fv(-1, -1);
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
    this->dirAngle = AngleToIndex(angle, &dirIndex);
    if(dirIndex != this->dirIndex){
        this->dirIndex = dirIndex;
        CX_METHOD_RUN(this->NodeDirection,this);
        cxFloat angle = dirIndex * 45.0f;
        cxViewSetDegrees(this->array, angle);
    }
}

//到达指定位置
static void NodeMoveArrive(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Node node = CX_TYPE_CAST(Node, cxActionView(this));
    cxHash bindes = cxViewBindes(node);
    CX_HASH_FOREACH(bindes, ele, tmp){
        Node target = cxHashElementKeyToAny(ele);
        if(NodeCheckDie(target)){
            continue;
        }
        //回调处理返回
        cxBool ret = CX_METHOD_GET(true, node->MoveExit, node, target);
        if(!ret){
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

cxRange2f NodeRange(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->type.range;
}

void NodeMovingToTarget(cxAny pview,cxAny target, cxAnyArray points)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeMap(this);
    //Test show point position
    cxList subviews = cxViewSubViews(map);
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
        cxViewSetColor(sp, cxYELLOW);
        cxViewSetPos(sp, pos);
        cxViewSetSize(sp, cxSize2fv(8, 8));
        cxViewSetTag(sp, 1001);
        cxViewAppend(map, sp);
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
    if(cxHashLength(bindes) >= this->attackNum){
        return;
    }
    //如果未定义搜索
    if(this->orders.number == 0){
        return;
    }
    if(this->searchIndex >= this->orders.number){
        this->searchIndex = 0;
    }
    //获取当前搜索类型
    NodeCombined type = this->orders.types[this->searchIndex++];
    Node target = MapNearestQuery(this, type);
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
    this->searchTimer = cxEngineCreateObject("fights.json?searchTimer");
    CX_EVENT_APPEND(this->searchTimer->onArrive, NodeSearchArrive);
    cxViewAppendAction(this, this->searchTimer);
}

void NodeSetLife(cxAny pview,cxInt life)
{
    CX_ASSERT_THIS(pview, Node);
    this->life = cxRange2iv(life, life);
    CX_EVENT_FIRE(this, onLife);
}

void NodeAddLife(cxAny pview,cxInt life)
{
    CX_ASSERT_THIS(pview, Node);
    this->life.min += life;
    CX_EVENT_FIRE(this, onLife);
}

void NodeSetLevel(cxAny pview,cxInt level)
{
    CX_ASSERT_THIS(pview, Node);
    this->level = level;
}

void NodeSetBody(cxAny pview,cxFloat body)
{
    CX_ASSERT_THIS(pview, Node);
    this->body  = body;
}

void NodeSetType(cxAny pview,NodeType type)
{
    CX_ASSERT_THIS(pview, Node);
    this->type.mainType = type;
}

void NodeSetSubType(cxAny pview,NodeSubType subType)
{
    CX_ASSERT_THIS(pview, Node);
    this->type.subType = subType;
}

cxFloat NodeAttackRate(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->attackRate;
}

void NodeSetAttackRate(cxAny pview,cxFloat rate)
{
    CX_ASSERT_THIS(pview, Node);
    this->attackRate = rate;
}

cxFloat NodePower(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->power * this->attackRate;
}

void NodeSetPower(cxAny pview,cxFloat power)
{
    CX_ASSERT_THIS(pview, Node);
    this->power = power;
}

void NodeSetSpeed(cxAny pview,cxFloat speed)
{
    CX_ASSERT_THIS(pview, Node);
    this->speed = speed;
}

void NodeSetRange(cxAny pview,cxRange2f range)
{
    CX_ASSERT_THIS(pview, Node);
    this->type.range = range;
}

cxAny NodeMap(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->map;
}

cxBool NodeIsStatic(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->isStatic;
}

cxBool NodeCheckDie(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    cxBool die = (this->life.min <= 0);
    if(die && !this->isDie){
        CX_LOGGER("node die at(%f %f)",this->index.x,this->index.y);
        this->isDie = true;
        this->life.min = 0;
        CX_EVENT_FIRE(this, onLife);
        CX_EVENT_FIRE(this, onDie);
        cxViewUnBindAll(pview);
        MapRemoveNode(pview);
    }
    return die;
}

cxRange2i NodeLife(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->life;
}

cxInt NodeLevel(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->level;
}

cxVec2f NodeFloatIndex(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->index;
}

cxSize2i NodeSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return cxSize2iv(this->size.w, this->size.h);
}

void NodeSetIndex(cxAny pview,cxVec2i idx)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    //为了对齐格子加上node大小
    cxVec2f fidx = cxVec2fv(idx.x + this->size.w/2.0f, idx.y + this->size.h/2.0f);
    
    cxVec2f npos = MapIndexToPos(map, fidx);
    cxViewSetPos(this, npos);
    //获取精确的格子坐标
    this->index = MapPosToFloat(map, npos);
}

void NodeSetSize(cxAny pview,cxSize2i size)
{
    CX_ASSERT_THIS(pview, Node);
    cxSize2f vsize = cxSize2fv(global.unitSize.w * size.w, global.unitSize.h * size.h);
    cxViewSetSize(this, vsize);
    this->size = size;
}

void NodeInit(cxAny pview,cxAny map,cxVec2i idx,cxBool isStatic)
{
    CX_ASSERT_THIS(pview, Node);
    this->map = map;
    this->initIdx = idx;
    this->isStatic = isStatic;
    NodeSetIndex(this, idx);
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













