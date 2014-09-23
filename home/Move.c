//
//  Move.c
//  Home
//
//  Created by xuhua on 9/22/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "Node.h"
#include "Move.h"
#include "Map.h"

static void MoveSetNextIndex(Move this)
{
    this->index ++;
    cxInt len = cxAnyArrayLength(this->points);
    if(this->index >= len){
        cxActionStop(this);
        return;
    }
    Node node = CX_TYPE_CAST(Node, this->cxAction.view);
    Map map = NodeMap(node);
    cxVec2f *to = cxAnyArrayAt(this->points, this->index, cxVec2f);
    this->to = MapIdxToPos(map, *to);
    kmVec2Subtract(&this->delta, &this->to, &this->from);
    this->angle = cxVec2fAngle(this->delta);
    CX_EVENT_FIRE(this, OnAngle);
}

void MoveSetTarget(cxAny pav,cxAny pview)
{
    CX_ASSERT_THIS(pav, Move);
    CX_RETAIN_SWAP(this->target, pview);
}

static void MoveInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    CX_ASSERT(this->target != this->cxAction.view,"target error");
    cxInt num = cxAnyArrayLength(this->points);
    if(num < 2){
        cxActionStop(pav);
        return;
    }
    Node node = CX_TYPE_CAST(Node, this->cxAction.view);
    NodeSetState(node, NodeStateMove);
    this->index = 0;
    this->from = cxViewPosition(this->cxAction.view);
    MoveSetNextIndex(this);
}

static void MoveStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, Move);
    Node node = CX_TYPE_CAST(Node, this->cxAction.view);
    this->from = cxViewPosition(node);
    this->from.x += this->speed * dt * cosf(this->angle);
    this->from.y += this->speed * dt * sinf(this->angle);
    cxVec2f delta;
    kmVec2Subtract(&delta, &this->to, &this->from);
    //如果方向相反
    if(cxVec2fIsInverse(delta,this->delta)){
        this->from = this->to;
        MoveSetNextIndex(this);
    }
    cxViewSetPos(node, this->from);
    CX_EVENT_FIRE(this, OnMoving);
}

CX_OBJECT_TYPE(Move, cxAction)
{
    
}
CX_OBJECT_INIT(Move, cxAction)
{
    CX_METHOD_SET(this->cxAction.Init, MoveInit);
    CX_METHOD_SET(this->cxAction.Step, MoveStep);
    this->index = 0;
    this->speed = 200;
    this->cxAction.time = -1;
    this->points = cxAnyArrayAlloc(cxVec2f);
    
    cxActionSetGroup(this, "fight");
}
CX_OBJECT_FREE(Move, cxAction)
{
    CX_EVENT_RELEASE(this->OnMoving);
    CX_EVENT_RELEASE(this->OnAngle);
    CX_RELEASE(this->target);
    CX_RELEASE(this->points);
}
CX_OBJECT_TERM(Move, cxAction)

void MoveAppendArray(cxAny pav,cxAnyArray points)
{
    CX_ASSERT_TYPE(points, cxAnyArray);
    CX_ANY_ARRAY_FOREACH(points, point, cxVec2i){
        MoveAppendPoint(pav, cxVec2fv(point->x, point->y));
    }
}

void MoveAppendPoint(cxAny pav,cxVec2f point)
{
    CX_ASSERT_THIS(pav, Move);
    cxAnyArrayAppend(this->points, &point);
}

