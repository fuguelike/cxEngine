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
    cxAny target;
    cxVec2f opos;
    cxFloat angle;
    cxFloat speed;  //current speed
    cxFloat init;   //init speed
CX_END(cxFollow, cxAction)

cxAny cxFollowTarget(cxAny pav);

void cxFollowSetInit(cxAny pav,cxFloat init);

void cxFollowSetTarget(cxAny pav,cxAny target);

cxFollow cxFollowCreate(cxFloat initSpeed,cxAny target);

CX_C_END

#endif
