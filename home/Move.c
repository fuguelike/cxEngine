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
    CX_ASSERT_VALUE(cxActionGetView(this), Node, node);
    NodeSetDirAngle(node, this->cxSpline.angle);
}

static void MoveOnInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, Move);
    CX_ASSERT_VALUE(cxActionGetView(this), Node, node);
    CX_ASSERT(MoveGetType(this) != MoveTypeNone, "not set move type");
    cxFloat d = 0;
    cxAnyArray points = this->cxSpline.points;
    cxInt num = cxAnyArrayLength(points);
    if(num < 2){
        cxActionStop(pav);
        return;
    }
    //计算总长度和时间
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
    CX_ASSERT_VALUE(cxActionGetView(this), Node, attacker);
    //战斗移动才检测target
    if(MoveGetType(this) != MoveTypeFight){
        return;
    }
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

static void MoveOnExit(cxAny pav)
{
    //node move exit
}

CX_OBJECT_TYPE(Move, cxSpline)
{
    
}
CX_OBJECT_INIT(Move, cxSpline)
{
    MoveSetIsToPoints(this, false);
    cxActionSetGroup(this, FIGHT_ACTION_GROUP);
    cxActionSetActionId(this, NodeActionMoveId);
    MoveSetType(this, MoveTypeNone);
    CX_ADD(cxSpline, this, onAngle, MoveOnAngle);
    CX_ADD(cxAction, this, onInit, MoveOnInit);
    CX_ADD(cxAction, this, onUpdate, MoveOnUpdate);
    CX_ADD(cxAction, this, onExit, MoveOnExit);
}
CX_OBJECT_FREE(Move, cxSpline)
{
    
}
CX_OBJECT_TERM(Move, cxSpline)

Move MoveCreate(cxAny node, cxAnyArray points)
{
    CX_ASSERT_THIS(node, Node);
    //移动创建前必须bind目标
    CX_ASSERT_TYPE(cxViewBindesFirst(this), Node);
    Map map = NodeGetMap(this);
    Move move = CX_CREATE(Move);
    cxVec2i vidx = NodeGetIntIndex(node);
    cxVec2f pos;
    CX_ASTAR_POINTS_FOREACH(points, idx){
        //如果在同一个网格位置则使用当前位置
        if(cxVec2iEqu(vidx, *idx)){
            pos = cxViewGetPosition(node);
        }else{
            //+0.5表示是格子的中心点
            pos = MapIndexToPos(map,cxVec2fv(idx->x + 0.5f, idx->y + 0.5f));
        }
        cxSplineAppend(move, pos);
    }
    return move;
}












