//
//  cxTimer.h
//  cxEngine
//
//  Created by xuhua on 10/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTimer_h
#define cxEngine_cxTimer_h

#include <engine/cxAction.h>

CX_C_BEGIN

#define CX_TIMER_FOREVER   -1

CX_OBJECT_DEF(cxTimer, cxAction)
    cxInt repeat;
    CX_EVENT_ALLOC(onArrive);
CX_OBJECT_END(cxTimer)

void cxTimerReset(cxAny timer);

cxTimer cxTimerCreate(cxFloat time,cxInt repeat);

CX_C_END

#endif
