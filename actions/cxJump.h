//
//  cxJump.h
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxJump_h
#define cxEngine_cxJump_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_DEF(cxJump, cxAction)
    cxVec2f startPos;
    cxVec2f prevPos;
    cxVec2f position;
    cxFloat height;
    cxInt  jumps;
CX_END(cxJump, cxAction)

cxJump cxJumpCreate(cxFloat time,cxVec2f position,cxFloat height,cxInt jumps);

CX_C_END

#endif
