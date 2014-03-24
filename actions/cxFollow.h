//
//  cxFollow.h
//  cxEngine
//
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFollow_h
#define cxEngine_cxFollow_h

#include <core/cxAction.h>

CX_C_BEGIN

typedef cxBool (cxFollowExitFunc)(cxAny pav);

CX_OBJECT_DEF(cxFollow, cxAction)
    cxAny target;
    cxFloat speed;
    cxFloat minDistance;
    CX_METHOD_ALLOC(cxBool, FollowExit, cxAny);
CX_OBJECT_END(cxFollow)

cxFollow cxFollowCreate(cxFloat speed,cxAny target);

CX_C_END

#endif
