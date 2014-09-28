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

static void MoveOnAngle(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    CX_ASSERT_VALUE(cxActionView(this), Node, node);
    NodeSetDirAngle(node, this->cxSpline.angle);
}

static void MoveOnInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    CX_ASSERT_VALUE(cxActionView(this), Node, node);
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
    CX_ASSERT(NodeGetSpeed(node) > 0, "node speed not set");
    cxActionSetTime(pav, d / NodeGetSpeed(node));
}

static void MoveOnUpdate(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    CX_ASSERT_VALUE(cxActionView(this), Node, attacker);
    //第一个bind的一定是目标,如果为空目标可能已经死亡
    Node target = cxViewBindesFirst(attacker);
    if(target == NULL){
        cxActionStop(this);
        return;
    }
    CX_ASSERT_TYPE(target, Node);
    //如果目标死亡停止移动
    if(NodeCheckDie(target)){
        cxActionStop(pav);
        return;
    }
    //到达作战范围
    if(NodeIsArriveRange(attacker, target)){
        cxActionStop(pav);
        return;
    }
}

CX_OBJECT_TYPE(Move, cxSpline)
{
    
}
CX_OBJECT_INIT(Move, cxSpline)
{
    MoveSetIsToPoints(this, false);
    cxActionSetGroup(this, "fight");
    CX_EVENT_APPEND(CX_TYPE(cxSpline, this)->onAngle, MoveOnAngle);
    CX_EVENT_APPEND(CX_TYPE(cxAction, this)->onInit, MoveOnInit);
    CX_EVENT_APPEND(CX_TYPE(cxAction, this)->onUpdate, MoveOnUpdate);
}
CX_OBJECT_FREE(Move, cxSpline)
{
    
}
CX_OBJECT_TERM(Move, cxSpline)

Move MoveCreate(cxAny pmap,cxAnyArray points)
{
    Move move = CX_CREATE(Move);
    CX_ASTAR_POINTS_FOREACH(points, idx){
        cxVec2f p = cxVec2fv(idx->x + 0.5f, idx->y + 0.5f);
        cxVec2f pos = MapIndexToPos(pmap,p);
        cxSplineAppend(move, pos);
    }
    return move;
}


