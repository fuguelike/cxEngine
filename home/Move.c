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

static void MoveOnIndex(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    //如果目标死亡停止移动
    Node target = CX_TYPE_CAST(Node, this->target);
    if(NodeIsDie(target)){
        cxActionStop(pav);
    }
}

static void MoveOnAngle(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    cxInt index = -1;
    cxFloat angle = AngleToIndex(this->cxSpline.angle, &index);
    if(this->index != index){
        this->angle = angle;
        this->index = index;
        CX_EVENT_FIRE(this, OnDegrees);
    }
}

static void MoveOnExit(cxAny pav)
{
    Node node = cxActionView(pav);
    node->isArrive = true;
}

static void MoveOnInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    cxFloat d = 0;
    cxAnyArray points = this->cxSpline.points;
    cxInt num = cxAnyArrayLength(points);
    if(num < 2){
        cxActionStop(pav);
        return;
    }
    cxVec2f *p1 = cxAnyArrayAt(points, 0, cxVec2f);
    for(cxInt i=1; i < num;i++){
        cxVec2f *p2 = cxAnyArrayAt(points, i, cxVec2f);
        d += kmVec2DistanceBetween(p1, p2);
        p1 = p2;
    }
    cxActionSetTime(pav, d / this->speed);
}

CX_OBJECT_TYPE(Move, cxSpline)
{
    
}
CX_OBJECT_INIT(Move, cxSpline)
{
    this->speed = 100;
    this->index = -1;
    this->angle = 0;
    cxActionSetGroup(this, "fight");
    CX_EVENT_APPEND(CX_TYPE(cxSpline, this)->onIndex, MoveOnIndex);
    CX_EVENT_APPEND(CX_TYPE(cxSpline, this)->onAngle, MoveOnAngle);
    CX_EVENT_APPEND(CX_TYPE(cxAction, this)->onExit, MoveOnExit);
    CX_EVENT_APPEND(CX_TYPE(cxAction, this)->onInit, MoveOnInit);
}
CX_OBJECT_FREE(Move, cxSpline)
{
    CX_EVENT_RELEASE(this->OnDegrees);
    CX_RELEASE(this->target);
}
CX_OBJECT_TERM(Move, cxSpline)

Move MoveCreate(cxAny target,cxAnyArray points)
{
    Node node = CX_TYPE_CAST(Node, target);
    Move this = CX_CREATE(Move);
    this->speed = node->speed;
    MoveSetTarget(this, target);
    Map map = NodeMap(target);
    CX_ANY_ARRAY_FOREACH(points, idx, cxVec2i){
        cxVec2f pos = MapIdxToPos(map,cxVec2fv(idx->x, idx->y));
        cxSplineAppend(this, pos);
    }
    return this;
}

void MoveSetTarget(cxAny pav,cxAny target)
{
    CX_ASSERT_THIS(pav, Move);
    CX_RETAIN_SWAP(this->target, target);
}

