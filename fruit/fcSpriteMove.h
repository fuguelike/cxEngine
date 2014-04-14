//
//  fcSpriteMove.h
//  cxEngine
//  精灵移动Action
//  Created by xuhua on 3/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcSpriteMove_h
#define fruit_fcSpriteMove_h

#include <actions/cxFollow.h>

CX_C_BEGIN

//移动
CX_OBJECT_DEF(fcSpriteMove, cxFollow)
    cxInt pointIndex;
    cxFloat speed;//提升的速度
    cxVec2f *points;
CX_OBJECT_END(fcSpriteMove)

cxAny fcSpriteMoveAction(cxAny pview);

void fcSpriteMoveStop(cxAny sprite);

//设置速度，可加速或减速
void fcSpriteMoveSetSpeed(cxAny sprite,cxFloat speed);

//向终点前进
void fcSpriteMoveLoop(cxAny sprite);

CX_C_END

#endif
