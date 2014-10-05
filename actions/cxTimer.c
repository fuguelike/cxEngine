//
//  cxTimer.c
//  cxEngine
//
//  Created by xuhua on 10/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTimer.h"

static void cxTimerInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxTimer);
    CX_RETURN(!this->IsBegin);
    CX_EVENT_FIRE(this, onArrive);
}

static cxBool cxTimerExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxTimer);
    this->Repeat --;
    CX_RETURN(this->Repeat == 0 && this->IsBegin, true);
    CX_EVENT_FIRE(this, onArrive);
    return this->Repeat == 0;
}

CX_SETTER_DEF(cxTimer, repeat)
{
    this->IsBegin = cxJsonToInt(value, this->Repeat);
}
CX_SETTER_DEF(cxTimer, begin)
{
    this->IsBegin = cxJsonToBool(value, this->IsBegin);
}

CX_OBJECT_TYPE(cxTimer, cxAction)
{
    CX_PROPERTY_SETTER(cxTimer, repeat);
    CX_PROPERTY_SETTER(cxTimer, begin);
}
CX_OBJECT_INIT(cxTimer, cxAction)
{
    this->IsBegin = false;
    CX_SET(cxAction, this, Init, cxTimerInit);
    CX_SET(cxAction, this, Exit, cxTimerExit);
}
CX_OBJECT_FREE(cxTimer, cxAction)
{
    CX_EVENT_RELEASE(this->onArrive);
}
CX_OBJECT_TERM(cxTimer, cxAction)

cxTimer cxTimerCreate(cxFloat freq,cxInt repeat)
{
    cxTimer this = CX_CREATE(cxTimer);
    this->cxAction.Time  = freq;
    cxTimerSetRepeat(this, repeat);
    return this;
}

