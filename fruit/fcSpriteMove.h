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

//移动
CX_OBJECT_DEF(fcSpriteMove, cxFollow)
    cxInt pointIndex;
    cxFloat speed;//提升的速度
CX_OBJECT_END(fcSpriteMove)

cxAny fcSpriteMoveAction(cxAny pview);

void fcSpriteMoveLoop(cxAny sprite,cxVec2i d);

CX_C_END

#endif
