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
    cxInt len = cxAnyArrayLength(this->points);
    if(this->index >= len){
        cxActionStop(this);
        return;
    }
    Node node = CX_TYPE_CAST(Node, this->cxAction.view);
    Map map = node->map;
    cxVec2f *to = cxAnyArrayAt(this->points, this->index, cxVec2f);
    this->to = MapIdxToPos(map, *to);
    kmVec2Subtract(&this->delta, &this->to, &this->from);
    this->angle = cxVec2fAngle(this->delta);
    CX_LOGGER("angle %f",kmRadiansToDegrees(this->angle));
}

static void MoveInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    cxInt num = cxAnyArrayLength(this->points);
    if(num < 2){
        cxActionStop(pav);
        return;
    }
    Node node = CX_TYPE_CAST(Node, this->cxAction.view);
    NodeSetState(node, NodeStateMove);
    this->index = 1;
    this->from = cxViewPosition(this->cxAction.view);
    MoveSetNextIndex(this);
}

static void MoveStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, Move);
    Node node = CX_TYPE_CAST(Node, this->cxAction.view);
    this->from = cxViewPosition(node);
    cxFloat d = kmVec2DistanceBetween(&this->from, &this->to);
    cxVec2f pos = cxViewPosition(node);
    pos.x += this->speed * dt * cosf(this->angle);
    pos.y += this->speed * dt * sinf(this->angle);
    if(d < global.unitSize.w){
        this->index ++;
        MoveSetNextIndex(this);
    }else{
        cxViewSetPos(node, pos);
    }
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
}
CX_OBJECT_FREE(Move, cxAction)
{
    CX_RELEASE(this->points);
}
CX_OBJECT_TERM(Move, cxAction)

void MoveAppendPoint(cxAny pav,cxVec2f point)
{
    CX_ASSERT_THIS(pav, Move);
    cxAnyArrayAppend(this->points, &point);
}

