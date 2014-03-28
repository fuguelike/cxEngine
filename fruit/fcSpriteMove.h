//
//  fcSpriteMove.h
//  cxEngine
//
//  Created by xuhua on 3/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcSpriteMove_h
#define fruit_fcSpriteMove_h

#include <actions/cxFollow.h>

CX_C_BEGIN

#define FC_MOVE_ACTION_ID   1000

//移动
CX_OBJECT_DEF(fcSpriteMove, cxFollow)
    cxInt pointIndex;
CX_OBJECT_END(fcSpriteMove)

void fcSpriteMoveLoop(cxAny sprite,cxVec2i d);

CX_C_END

#endif
