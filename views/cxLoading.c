//
//  cxLoading.c
//  cxEngineIOS
//
//  Created by xuhua on 11/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <actions/cxTimer.h>
#include "cxLoading.h"

void cxLoadingOnUpdate(cxAny sender)
{
    cxLoading this = sender;
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

static void cxFinishedArrive(cxAny sender)
{
    cxLoading this = cxActionView(sender);
    cxLoaingFinished(this);
}

static void cxLoadingArrive(cxAny sender)
{
    cxLoading this = cxActionView(sender);
    CX_EVENT_FIRE(this, onLoading);
    if(this->autoFinished){
        cxTimer timer = cxViewAppendTimer(this, 1.0f, 1);
        CX_EVENT_APPEND(timer->onArrive, cxFinishedArrive);
    }
}

void cxLoaingFinished(cxAny pview)
{
    cxLoading this = pview;
    this->isLoading = false;
}

CX_OBJECT_TYPE(cxLoading, cxView)
{
    
}
CX_OBJECT_INIT(cxLoading, cxView)
{
    this->autoFinished = true;
    this->isLoading = true;
    CX_EVENT_APPEND(this->cxView.onUpdate, cxLoadingOnUpdate);
    CX_METHOD_SET(this->cxView.Touch, cxLoadingTouch);
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
    CX_EVENT_APPEND(timer->onArrive, cxLoadingArrive);
    cxViewAppend(engine->window, this);
}


