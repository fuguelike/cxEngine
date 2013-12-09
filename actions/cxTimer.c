//
//  cxTimer.c
//  cxEngine
//
//  Created by xuhua on 10/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxActionXML.h>
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

static void cxTimerXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionXMLReadAttr(xmlAction, mAction, reader);
    cxActionXML xml = xmlAction;
    cxTimer this = mAction;
    this->repeat = cxXMLReadIntAttr(reader, "cxTimer.repeat", this->repeat);
    cxXMLAppendEvent(xml->events, this, cxTimer, onArrive);
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
    cxObjectSetXMLReadFunc(this, cxTimerXMLReadAttr);
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