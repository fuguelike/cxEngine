//
//  cxTimer.c
//  cxEngine
//
//  Created by xuhua on 10/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxActionRoot.h>
#include "cxTimer.h"

static void cxTimerInit(cxAny pav)
{
    cxTimer this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    CX_UNUSED_PARAM(this);
}

static void cxTimerStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxTimer this = pav;
    CX_UNUSED_PARAM(this);
}

static void cxTimerReadAttr(cxAny rootAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionReadAttr(rootAction, mAction, reader);
    cxActionRoot root = rootAction;
    cxTimer this = mAction;
    this->repeat = cxXMLReadIntAttr(reader, root->functions, "cxTimer.repeat", this->repeat);
    cxXMLAppendEvent(root->events, this, cxTimer, onArrive);
}

static cxBool cxTimerExit(cxAny pav)
{
    cxTimer this = pav;
    if(this->repeat > 0){
        this->repeat --;
    }
    CX_EVENT_FIRE(this, onArrive);
    return this->repeat == 0;
}

CX_OBJECT_INIT(cxTimer, cxAction)
{
    cxActionSetStepHide(this, true);
    cxObjectSetReadAttrFunc(this, cxTimerReadAttr);
    CX_METHOD_SET(this->super.Init, cxTimerInit);
    CX_METHOD_SET(this->super.Step, cxTimerStep);
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