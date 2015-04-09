//
//  cxFollow.h
//  cxEngine
//
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFollow_h
#define cxEngine_cxFollow_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_DEF(cxFollow, cxAction)
    CX_FIELD_DEF(cxFloat Angle);
    cxAny target;
    cxVec2f opos;
    cxFloat speed;
CX_END(cxFollow, cxAction)

CX_FIELD_GET(cxFollow, cxFloat, Angle);

cxAny cxFollowTarget(cxAny pav);

void cxFollowInit(cxAny pav,cxAny pview,cxFloat speed,cxAny target);

//action view must bind target
cxFollow cxFollowCreate(cxFloat speed,cxAny pview, cxAny target);

CX_C_END

#endif
