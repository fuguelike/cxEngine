//
//  cxLoading.c
//  cxEngineIOS
//
//  Created by xuhua on 11/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <actions/cxTimer.h>
#include "cxLoading.h"

void cxLoadingOnUpdate(cxEvent *event)
{
    cxLoading this = event->sender;
    if(!this->isLoading){
        CX_EVENT_FIRE(this, onFinished);
        cxViewRemoved(this);
    }
}

cxBool cxLoadingTouch(cxAny pview,cxTouch *touch)
{
    return true;
}

void cxLoadingSetObject(cxAny pview,cxAny object)
{
    cxLoading this = pview;
    CX_RETAIN_SWAP(this->object, object);
}

cxAny cxLoadingObject(cxAny pview)
{
    cxLoading this = pview;
    return this->object;
}

static void cxFinishedArrive(cxEvent *event)
{
    cxLoading this = cxActionView(event->sender);
    cxLoaingFinished(this);
}

static void cxLoadingArrive(cxEvent *event)
{
    cxLoading this = cxActionView(event->sender);
    CX_EVENT_FIRE(this, onLoading);
    cxTimer timer = cxViewAppendTimer(this, 1.0f, 1);
    CX_EVENT_QUICK(timer->onArrive, cxFinishedArrive);
}

void cxLoaingFinished(cxAny pview)
{
    cxLoading this = pview;
    this->isLoading = false;
}

CX_OBJECT_INIT(cxLoading, cxView)
{
    this->isLoading = true;
    CX_EVENT_QUICK(this->super.onUpdate, cxLoadingOnUpdate);
    CX_METHOD_OVERRIDE(this->super.Touch, cxLoadingTouch);
}
CX_OBJECT_FREE(cxLoading, cxView)
{
    CX_EVENT_RELEASE(this->onStart);
    CX_EVENT_RELEASE(this->onFinished);
    CX_EVENT_RELEASE(this->onLoading);
}
CX_OBJECT_TERM(cxLoading, cxView)

void cxLoadingStart(cxLoading this)
{
    cxEngine engine = cxEngineInstance();
    CX_EVENT_FIRE(this, onStart);
    this->isLoading = true;
    cxTimer timer = cxViewAppendTimer(this, 1.0f, 1);
    CX_EVENT_QUICK(timer->onArrive, cxLoadingArrive);
    cxViewAppend(engine->window, this);
}


