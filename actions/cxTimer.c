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
    CX_RETURN(!this->isBegin);
    CX_EVENT_FIRE(this, onArrive);
}

static cxBool cxTimerExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxTimer);
    this->repeat --;
    CX_RETURN(this->repeat == 0 && this->isBegin, true);
    CX_EVENT_FIRE(this, onArrive);
    return this->repeat == 0;
}

CX_SETTER_DEF(cxTimer, repeat)
{
    this->repeat = cxJsonToInt(value, this->repeat);
}
CX_SETTER_DEF(cxTimer, begin)
{
    this->isBegin = cxJsonToBool(value, this->isBegin);
}

CX_OBJECT_TYPE(cxTimer, cxAction)
{
    CX_PROPERTY_SETTER(cxTimer, repeat);
    CX_PROPERTY_SETTER(cxTimer, begin);
}
CX_OBJECT_INIT(cxTimer, cxAction)
{
    this->isBegin = false;
    CX_METHOD_SET(this->cxAction.Init, cxTimerInit);
    CX_METHOD_SET(this->cxAction.Exit, cxTimerExit);
}
CX_OBJECT_FREE(cxTimer, cxAction)
{
    CX_EVENT_RELEASE(this->onArrive);
}
CX_OBJECT_TERM(cxTimer, cxAction)

void cxTimerSetBegin(cxAny pav,cxBool isBegin)
{
    CX_ASSERT_THIS(pav, cxTimer);
    this->isBegin = isBegin;
}

cxTimer cxTimerCreate(cxFloat freq,cxInt repeat)
{
    cxTimer this = CX_CREATE(cxTimer);
    this->cxAction.time  = freq;
    this->repeat = repeat;
    return this;
}

