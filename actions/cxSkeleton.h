//
//  cxSkeleton.h
//  cxEngine
//  use at cxSpine
//  Created by xuhua on 9/14/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxSkeleton_h
#define cxEngine_cxSkeleton_h

#include <spine/spine.h>
#include <engine/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxSkeleton, cxAction)
    cxFloat time;
    spAnimationState *state;
    cxJson tracks;
CX_OBJECT_END(cxSkeleton, cxAction)

CX_C_END

#endif
