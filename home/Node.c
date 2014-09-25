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
#include <types/Bullet.h>
#include "Node.h"
#include "Move.h"
#include "Map.h"

cxVec2f NodePosToIdx(cxAny pview,cxVec2f pos)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxSize2f vsize = cxViewSize(this);
    pos.y -= (vsize.h / 2.0f - (vsize.h / (2.0f * this->size.h)));
    return MapPosToIdx(map, pos);
}

static void MapNodesEachSelected(cxAny ps,cxAny pview,cxAny data)
{
    Node node = CX_TYPE_CAST(Node, pview);
    if(node == data){
        //选中
        cxViewSetColor(node, cxYELLOW);
    }else{
        //未选中
        cxViewSetColor(node, cxRED);
    }
}
static cxBool NodeSelected(Node this)
{
    Map map = this->map;
    cxSpatialEach(map->items, MapNodesEachSelected, this);
    map->node = this;
    return true;
}

static void NodeMoveIsValid(Node this,cxBool valid)
{
    if(valid){
        cxViewSetColor(this, cxGREEN);
    }else{
        cxViewSetColor(this, cxGRAY);
    }
}

static cxBool NodeTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, Node);
    CX_RETURN(points->number != 1,false);
    cxTouchItem item = points->items[0];
    Map map = this->map;
    if(map->mode != MapModeNormal){
        return false;
    }
    if(!this->canSelected){
        return false;
    }
    cxVec2f cpos;
    cxBool hited = cxViewHitTest(pview, item->position, &cpos) && cxPointsContainPoint(this->box, cpos);
    if(item->type == cxTouchTypeMove &&  map->node == this && this->isTouch){
        cxVec2f vpos = this->start;
        cxVec2f delta = cxViewTouchDelta(pview, item);
        kmVec2Add(&vpos, &vpos, &delta);
        cxVec2f idx = NodePosToIdx(this,vpos);
        this->isValidIdx = NodeIdxIsValid(this, idx);
        NodeMoveIsValid(this,this->isValidIdx);
        NodeSetPosition(this, idx, false);
        this->start = vpos;
        return true;
    }
    if(item->type == cxTouchTypeDown){
        this->isTouch = hited;
        this->start = cxViewPosition(this);
        if(hited){
            cxViewBringFront(this);
        }
        return false;
    }
    if(item->type == cxTouchTypeUp && item->movement < 20){
        this->isSelected = hited;
        if(this->isSelected && map->node != NULL){
            NodeResetPosition(map->node);
        }
        if(this->isSelected){
            return NodeSelected(this);
        }
        return false;
    }
    return false;
}

cxBool NodeArriveAttack(cxAny psx,cxAny pdx)
{
    Node sx = CX_TYPE_CAST(Node, psx);
    Node dx = CX_TYPE_CAST(Node, pdx);
    cxVec2f p1 = cxViewPosition(sx);
    cxVec2f p2 = cxViewPosition(dx);
    return NodeArriveAttackWithPoint(psx, pdx, p1, p2);
}
cxBool NodeArriveAttackWithPoint(cxAny psx,cxAny pdx,cxVec2f p1,cxVec2f p2)
{
    Node sx = CX_TYPE_CAST(Node, psx);
    Node dx = CX_TYPE_CAST(Node, pdx);
    //到达攻击距离
    cxFloat d = kmVec2DistanceBetween(&p1, &p2) - (sx->body + dx->body) * global.sideLen;
    d = CX_MAX(d, 0);
    cxRange2f range = NodeRange(sx);
    cxFloat max = range.max * global.sideLen;
    cxFloat min = range.min * global.sideLen;
    return d >= min && d <= max;
}

cxVec2f NodePosIndex(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    cxVec2f pos = cxViewPosition(this);
    return NodePosToIdx(this, pos);
}

static void NodeOnTransform(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxVec2f idx = NodePosIndex(this);
    if(!cxVec2fEqu(this->idx, idx)){
        this->idx = idx;
        cxSpatialReindexView(map->items, this);
    }
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

void NodeMomentDie(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    this->life.min = 0;
    CX_EVENT_FIRE(this, onLife);
    CX_EVENT_FIRE(this, onDie);
    cxViewUnBindAll(pview);
    MapRemoveNode(pview);
}

cxVec2f NodeNearestPoint(cxAny pview,cxVec2f idx)
{
    CX_ASSERT_THIS(pview, Node);
    cxVec2f ret = this->idx;
    if(this->size.h == 1 && this->size.w == 1){
        return ret;
    }
    cxFloat dis = INT32_MAX;
    for(cxFloat x = this->idx.x; x < this->idx.x + this->size.w; x++)
    for(cxFloat y = this->idx.y; y < this->idx.y + this->size.h; y++){
        cxVec2f c = cxVec2fv(x, y);
        cxFloat d = kmVec2DistanceBetween(&c, &idx);
        //获取最近的点
        if(d < dis){
            ret = c;
            dis = d;
        }
    }
    return ret;
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

CX_OBJECT_TYPE(Node, cxSprite)
{
    
}
CX_OBJECT_INIT(Node, cxSprite)
{
    this->canSelected = true;
    this->idx = cxVec2fv(-1, -1);
    CX_METHOD_SET(this->cxSprite.cxView.Touch, NodeTouch);
    CX_EVENT_APPEND(CX_TYPE(cxView, this)->onTransform, NodeOnTransform);
    this->box = cxAnyArrayAlloc(cxVec2f);
    this->speed = 100;
    this->attackNum = 1;
    this->searchIndex = 0;
    this->dirIndex = 0;
    CX_METHOD_SET(this->NodeAttacked, NodeAttacked);
}
CX_OBJECT_FREE(Node, cxSprite)
{
    CX_EVENT_RELEASE(this->onDie);
    CX_EVENT_RELEASE(this->onLife);
    CX_RELEASE(this->box);
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
        cxConstChars n = CX_CONST_STRING("%d.png",this->dirIndex);
        cxSpriteSetTextureURL(this, n);
    }
}

static void NodeMoveOnExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Node node = CX_TYPE_CAST(Node, cxActionView(this));
    cxHash bindes = cxViewBindes(node);
    CX_HASH_FOREACH(bindes, ele, tmp){
        cxNumber data = CX_TYPE_CAST(cxNumber, ele->any);
        Node target = cxHashElementKeyToAny(ele);
        if(NodeIsDie(target)){
            cxViewUnBind(node, target);
            continue;
        }
        //回调处理返回
        cxBool ret = CX_METHOD_GET(true, node->MoveExit, node, target);
        cxNumberSetBool(data, ret);
        if(!ret){
            cxViewUnBind(node, target);
            continue;
        }
        //朝向target
        NodeFaceTarget(node, target);
    }
}

cxRange2f NodeRange(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->type.range;
}

void NodeMoveTo(cxAny pview,cxAnyArray points)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeMap(this);
    //Test
    cxList subviews = cxViewSubViews(map);
    CX_LIST_FOREACH(subviews, ele){
        cxView tmp = ele->any;
        if(tmp->tag == 1001){
            cxViewRemove(tmp);
        }
    }
    CX_ASTAR_POINTS_FOREACH(points, idx){
        cxVec2f p = cxVec2fv(idx->x, idx->y);
        cxVec2f pos = MapIdxToPos(map, p);
        cxSprite sp = cxSpriteCreateWithURL("bullet.json?shell.png");
        cxViewSetColor(sp, cxYELLOW);
        cxViewSetPos(sp, pos);
        cxViewSetSize(sp, cxSize2fv(10, 10));
        cxViewSetTag(sp, 1001);
        cxViewAppend(map, sp);
    }
    
    Move move = MoveCreate(map, points);
    CX_EVENT_APPEND(CX_TYPE(cxAction, move)->onExit, NodeMoveOnExit);
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
    isAttack = false;   //默认不设置攻击标识
    cxAny bind = CX_METHOD_GET(NULL,this->FindTarget,this,target, &isAttack);
    if(bind != NULL){
        cxViewBind(this, bind, cxNumberBool(isAttack));
    }
}

void NodeAttackTarget(cxAny attacker,cxAny target,AttackType type)
{
    CX_ASSERT_THIS(target, Node);
    CX_METHOD_RUN(this->NodeAttacked,this,attacker,type);
    if(NodeIsDie(this)){
        NodeMomentDie(target);
    }
}

static void NodeProcessAttack(Node this)
{
    cxHash bindes = cxViewBindes(this);
    CX_HASH_FOREACH(bindes, ele, tmp){
        Node target = cxHashElementKeyToAny(ele);
        //未设定标记无法攻击
        if(!cxNumberToBool(ele->any)){
            continue;
        }
        //超过攻击范围取消bind,将导致重新搜索目标
        if(!NodeArriveAttack(this, target)){
            cxViewUnBind(this, target);
            continue;
        }
        //攻击时处理攻击方式，直接攻击或者投掷物体攻击
        CX_METHOD_RUN(this->AttackTarget,this,target);
    }
}

//搜索附近的
static void NodeSearchArrive(cxAny pav)
{
    cxAny pview = cxActionView(pav);
    CX_ASSERT_THIS(pview, Node);
    NodeProcessSearch(this);
}

void NodeSetState(cxAny pview,NodeState state)
{
    CX_ASSERT_THIS(pview, Node);
    this->state = state;
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

//暂停攻击
void NodePauseAttack(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    CX_RETURN(this->attackTimer == NULL);
    cxActionPause(this->attackTimer);
}

//重新启动攻击
void NodeResumeAttack(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    CX_RETURN(this->attackTimer == NULL);
    cxActionReset(this->attackTimer);
    cxActionResume(this->attackTimer);
}

//搜索附近的
static void NodeAttackArrive(cxAny pav)
{
    cxAny pview = cxActionView(pav);
    CX_ASSERT_THIS(pview, Node);
    NodeProcessAttack(this);
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

void NodeSetAttackRate(cxAny pview,cxFloat rate)
{
    CX_ASSERT_THIS(pview, Node);
    this->attackRate = rate;
    if(rate > 0){
        NodeAttackRun(this);
    }else{
        NodePauseAttack(this);
    }
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

cxBool NodeIsDie(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->life.min <= 0;
}

//启动攻击定时器
void NodeAttackRun(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    this->attackTimer = cxEngineCreateObject("fights.json?attackTimer");
    cxActionSetTime(this->attackTimer, this->attackRate);
    CX_EVENT_APPEND(this->attackTimer->onArrive, NodeAttackArrive);
    cxViewAppendAction(this, this->attackTimer);
}

cxBool NodeHasPoint(cxAny pview,cxVec2i idx)
{
    CX_ASSERT_THIS(pview, Node);
    return idx.x >= this->idx.x &&
    idx.x < (this->idx.x + this->size.w) &&
    idx.y >= this->idx.y &&
    idx.y < (this->idx.y + this->size.h);
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

cxVec2i NodeIndex(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return cxVec2iv(this->idx.x, this->idx.y);
}

cxSize2i NodeSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return cxSize2iv(this->size.w, this->size.h);
}

void NodeResetPosition(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    CX_RETURN(this->isValidIdx);
    NodeSetPosition(this, this->idx, true);
    this->isValidIdx = true;
}

cxBool NodeSetPosition(cxAny pview,cxVec2f idx,cxBool animate)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    this->curr = idx;
    idx.x += (this->size.w - 1.0f)/2.0f;
    idx.y += (this->size.h - 1.0f)/2.0f;
    cxVec2f npos = MapIdxToPos(map, idx);
    if(!animate){
        cxViewSetPos(this, npos);
    }else{
        cxMove m = cxMoveCreate(0.1f, npos);
        cxViewAppendAction(pview, m);
    }
    return true;
}

void NodeSetSize(cxAny pview,cxSize2f size)
{
    CX_ASSERT_THIS(pview, Node);
    cxSize2f vsize = cxSize2fv(global.unitSize.w * size.w, global.unitSize.h * size.h);
    cxViewSetSize(this, vsize);
    this->size = size;
    cxAnyArrayClean(this->box);
    cxAnyArrayAppend(this->box, &(cxVec2fv(vsize.w/2.0f, 0)));
    cxAnyArrayAppend(this->box, &(cxVec2fv(0, vsize.h/2.0f)));
    cxAnyArrayAppend(this->box, &(cxVec2fv(-vsize.w/2.0f, 0)));
    cxAnyArrayAppend(this->box, &(cxVec2fv(0, -vsize.h/2.0f)));
}

cxBool NodeIdxIsValid(cxAny pview,cxVec2f curr)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxVec2i idx = cxVec2iv(curr.x, curr.y);
    cxSize2i size = NodeSize(this);
    // 1 = max -1
    if(idx.x < MAP_BORDER || (idx.x + size.w) > (global.unitNum.x - MAP_BORDER)){
        return false;
    }
    if(idx.y < MAP_BORDER || (idx.y + size.h) > (global.unitNum.y - MAP_BORDER)){
        return false;
    }
    for(cxInt x = idx.x; x < idx.x + size.w; x ++)
    for(cxInt y = idx.y; y < idx.y + size.h; y++){
        cxAny item = MapNode(map, x, y);
        if(item != NULL && item != this){
            return false;
        }
    }
    return true;
}

cxVec2f NodeCurrIdx(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->curr;
}

void NodeUpdateIdx(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    NodeSetIdx(pview, this->curr);
}

void NodeSetIdx(cxAny pview,cxVec2f idx)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = this->map;
    cxVec2i nidx = cxVec2iv(idx.x, idx.y);
    cxVec2i oidx = NodeIndex(this);
    if(cxVec2iEqu(nidx, oidx)){
        return;
    }
    //攻击活动单位不放入A*搜索数据中
    if(this->type.mainType!= NodeTypeAttack){
        MapFillNode(map, nidx, this);
    }
    this->idx = idx;
    this->curr = idx;
    this->isValidIdx = true;
    MapNodeOnNewIdx(map, this);
}

void NodeInit(cxAny pview,cxAny map, cxSize2f size,cxVec2f idx)
{
    CX_ASSERT_THIS(pview, Node);
    this->map = map;
    NodeSetSize(this, size);
    NodeSetIdx(this, idx);
    NodeSetPosition(this, idx, false);
}