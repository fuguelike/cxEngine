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
#include "Node.h"
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
    if(pview == data){
        //选中
        cxViewSetColor(pview, cxYELLOW);
    }else{
        //未选中
        cxViewSetColor(pview, cxRED);
    }
}
static cxBool NodeSelected(Node this)
{
    Map map = this->map;
    cxSpatialEach(map->defences, MapNodesEachSelected, this);
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
    //战斗模式下
    if(map->mode != MapModeFight){
        return;
    }
    //只更新攻击单位位置
    if(this->type != NodeTypeAttack){
        return;
    }
    cxVec2f idx = NodePosIndex(this);
    if(!cxVec2fEqu(this->idx, idx)){
        this->idx = idx;
        cxSpatialReindexView(map->attacks, this);
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
}
CX_OBJECT_FREE(Node, cxSprite)
{
    CX_RELEASE(this->box);
}
CX_OBJECT_TERM(Node, cxSprite)

void NodeAppend(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    CX_ASSERT(this->Append != NULL, "not set append method");
    CX_METHOD_RUN(this->Append,this);
}

void NodeRemove(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    CX_ASSERT(this->Remove != NULL, "not set remove method");
    CX_METHOD_RUN(this->Remove,this);
}

static cpCollisionID NodeIndexQueryFunc(cxAny ps, cxAny pview, cpCollisionID id, void *data)
{
    NodeNearestInfo *info = data;
    Node node = CX_TYPE_CAST(Node, pview);
    //不匹配类型
    if(info->type != NodeTypeNone && !(info->type & node->type)){
        return id;
    }
    //不匹配子类型
    if(info->subType != NodeSubTypeNone && !(info->subType & node->subType)){
        return id;
    }
    //计算距离
    cxVec2f tidx = cxVec2fv(node->idx.x * global.sideLen, node->idx.y * global.sideLen);
    cxFloat d = kmVec2DistanceBetween(&info->idx, &tidx);
    if(d > info->dis){
        return id;
    }
    info->dis = d;
    //在范围内
    if(info->dis >= info->range.min && info->dis <= info->range.max){
        info->node = node;
    }
    return id;
}

static cpFloat NodeSegmentQueryFunc(cxAny ps, cxAny pview, void *data)
{
    NodeSegmentInfo *info = data;
    Node node = CX_TYPE_CAST(Node, pview);
    //不匹配类型
    if(info->type != NodeTypeNone && !(info->type & node->type)){
        return info->exit;
    }
    //不匹配子类型
    if(info->subType != NodeSubTypeNone && !(info->subType & node->subType)){
        return info->exit;
    }
    //计算a点距离
    cxVec2f tidx = cxVec2fv(node->idx.x * global.sideLen, node->idx.y * global.sideLen);
    cxFloat ad = kmVec2DistanceBetween(&info->a, &tidx);
    cxFloat ab = kmVec2DistanceBetween(&info->b, &tidx);
    if(ab > info->ab){
        return info->exit;
    }
    if(ad > info->dis){
        return info->exit;
    }
    info->dis = ad;
    info->node = node;
    return info->exit;
}

cxAny NodeSegment(cxAny ps,cxVec2f a,cxVec2f b,NodeType type,NodeSubType subType)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    NodeSegmentInfo ret = {0};
    ret.a = cxVec2fv(a.x * global.sideLen, a.y * global.sideLen);
    ret.b = cxVec2fv(b.x * global.sideLen, b.y * global.sideLen);
    ret.ab = kmVec2DistanceBetween(&ret.a, &ret.b);
    ret.dis = INT32_MAX;
    ret.type = type;
    ret.subType = subType;
    ret.exit = 1.0f;
    cpSpatialIndexSegmentQuery(this->index, this, cpv(ret.a.x, ret.a.y), cpv(ret.b.x, ret.b.y), ret.exit, NodeSegmentQueryFunc, &ret);
    return ret.node;
}

cxAny NodeNearest(cxAny ps,cxVec2f idx,cxRange2f range,NodeType type,NodeSubType subType)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    NodeNearestInfo ret = {0};
    ret.idx = cxVec2fv(idx.x * global.sideLen, idx.y * global.sideLen);
    ret.dis = INT32_MAX;
    ret.range = cxRange2fv(range.min * global.sideLen, range.max * global.sideLen);
    ret.type = type;
    ret.subType = subType;
    cpBB bb = cpBBNewForCircle(cpv(ret.idx.x,ret.idx.y), cpfmax(range.max * global.sideLen, 0.0f));
    cpSpatialIndexQuery(this->index, this, bb, NodeIndexQueryFunc, &ret);
    return ret.node;
}

//搜索附近的
static void NodeSearchArrive(cxAny pav)
{
    cxAny pview = cxActionView(pav);
    CX_ASSERT_THIS(pview, Node);
    CX_METHOD_RUN(this->Search,this);
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
    CX_METHOD_RUN(this->Attack,this);
}

void NodeSetLife(cxAny pview,cxInt life)
{
    CX_ASSERT_THIS(pview, Node);
    this->life = cxRange2iv(life, life);
    CX_METHOD_RUN(this->LifeChanged,this);
}

void NodeAddLife(cxAny pview,cxInt life)
{
    CX_ASSERT_THIS(pview, Node);
    this->life.min += life;
    CX_METHOD_RUN(this->LifeChanged,this);
}

void NodeSetLevel(cxAny pview,cxInt level)
{
    CX_ASSERT_THIS(pview, Node);
    this->level = level;
}

void NodeSetAttackRate(cxAny pview,cxFloat rate)
{
    CX_ASSERT_THIS(pview, Node);
    this->attackRate = rate;
    if(rate > 0){
        NodeAttackRun(this);
    }
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
    this->range = range;
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

cxBool NodeAtRange(cxAny pview,cxAny node)
{
    CX_ASSERT_THIS(pview, Node);
    Node snode = CX_TYPE_CAST(Node, node);
    cxFloat d = kmVec2DistanceBetween(&this->idx, &snode->idx);
    return (d >= this->range.min && d <= this->range.max);
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

cxRange2f NodeRange(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->range;
}

cxFloat NodePower(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    return this->power;
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
    for(cxInt x = idx.x; x < idx.x + size.w; x ++){
        for (cxInt y = idx.y; y < idx.y + size.h; y++) {
            cxAny item = MapNode(map, x, y);
            if(item != NULL && item != this){
                return false;
            }
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
    if(this->type != NodeTypeAttack){
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