//
//  Move.h
//  Home
//
//  Created by xuhua on 9/22/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Move_h
#define Home_Move_h

#include <engine/cxAction.h>

CX_C_BEGIN

//0(-d4) 30(-d3) 60(-d2) 90(d1) 120(d2) 150(d3) 180(d4) 210(d5) 240(d6) 270(d7) 300(-d6) 330(-d5)

CX_OBJECT_DEF(Move, cxAction)
    cxAnyArray points;
    cxInt index;
    cxVec2f delta;
    cxFloat speed;
    cxVec2f from;
    cxVec2f to;
    cxFloat angle;
    cxAny target;
    CX_EVENT_ALLOC(OnAngle);    //当角度变化时需要更换纹理动画
    CX_EVENT_ALLOC(OnMoving);   //当移动时
CX_OBJECT_END(Move, cxAction)

void MoveSetTarget(cxAny pav,cxAny pview);

void MoveAppendPoint(cxAny pav,cxVec2f point);

void MoveAppendArray(cxAny pav,cxAnyArray points);

CX_C_END

#endif
