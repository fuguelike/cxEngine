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
    cxInt index = -1;
    this->angle = AngleToIndex(this->cxSpline.angle, &index);
    if(this->index != index){
        this->index = index;
        CX_EVENT_FIRE(this, OnDegrees);
    }
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

static void MoveOnUpdate(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    Node attacker = CX_TYPE_CAST(Node, cxActionView(this));
    Node target = CX_TYPE_CAST(Node, this->target);
    //如果目标死亡停止移动
    if(NodeIsDie(target)){
        cxActionStop(pav);
        return;
    }
    //如果到达攻击距离
    if(NodeArriveAttack(attacker, target)){
        cxActionStop(pav);
        return;
    }
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
    CX_EVENT_APPEND(CX_TYPE(cxSpline, this)->onAngle, MoveOnAngle);
    CX_EVENT_APPEND(CX_TYPE(cxAction, this)->onInit, MoveOnInit);
    CX_EVENT_APPEND(CX_TYPE(cxAction, this)->onUpdate, MoveOnUpdate);
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
    Map map = NodeMap(node);
    Move move = CX_CREATE(Move);
    //设置速度
    move->speed = node->speed;
    //设置目标
    MoveSetTarget(move, node);
    //加入路径点
    CX_ASTAR_POINTS_FOREACH(points, idx){
        cxVec2f pos = MapIdxToPos(map,cxVec2fv(idx->x, idx->y));
        cxSplineAppend(move, pos);
    }
    return move;
}

void MoveSetTarget(cxAny pav,cxAny target)
{
    CX_ASSERT_THIS(pav, Move);
    CX_RETAIN_SWAP(this->target, target);
}

