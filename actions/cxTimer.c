//
//  cxTimer.c
//  cxEngine
//
//  Created by xuhua on 10/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTimer.h"

static cxBool cxTimerExit(cxAny pav)
{
    cxTimer this = pav;
    this->repeat --;
    CX_EVENT_FIRE(this, onArrive);
    return this->repeat == 0;
}

void cxTimerReset(cxAny timer)
{
    cxTimer this = timer;
    this->super.durationElapsed = 0;
}

CX_SETTER_DEF(cxTimer, repeat)
{
    this->repeat = cxJsonToInt(value, this->repeat);
}

CX_OBJECT_TYPE(cxTimer, cxAction)
{
    CX_PROPERTY_SETTER(cxTimer, repeat);
}
CX_OBJECT_INIT(cxTimer, cxAction)
{
    CX_METHOD_SET(this->super.Exit, cxTimerExit);
}
CX_OBJECT_FREE(cxTimer, cxAction)
{
    CX_EVENT_RELEASE(this->onArrive);
}
CX_OBJECT_TERM(cxTimer, cxAction)

cxTimer cxTimerCreate(cxFloat time,cxInt repeat)
{
    cxTimer this = CX_CREATE(cxTimer);
    this->super.duration  = time;
    this->repeat = repeat;
    return this;
}

