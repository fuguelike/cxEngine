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
    if(this->isLoading){
        CX_METHOD_RUN(this->Finished,this->object);
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
    cxAny obj = CX_METHOD_GET(NULL, this->Loading, this);
    CX_ASSERT(obj != NULL, "loading must return view");
    cxLoadingSetObject(this, obj);
    
    cxTimer timer = cxViewAppendTimer(this, 1.0f, 1);
    CX_EVENT_QUICK(timer->onArrive, cxFinishedArrive);
}

void cxLoaingFinished(cxAny pview)
{
    cxLoading this = pview;
    this->isLoading = true;
}

CX_OBJECT_INIT(cxLoading, cxView)
{
    this->isLoading = false;
    CX_EVENT_QUICK(this->super.onUpdate, cxLoadingOnUpdate);
    CX_METHOD_SET(this->super.Touch, cxLoadingTouch);
}
CX_OBJECT_FREE(cxLoading, cxView)
{
    
}
CX_OBJECT_TERM(cxLoading, cxView)

cxAny cxLoadingStart(cxLoadingFunc loading,cxFinishedFunc finished)
{
    cxEngine engine = cxEngineInstance();
    
    cxLoading this = CX_CREATE(cxLoading);
    CX_METHOD_SET(this->Finished, finished);
    CX_METHOD_SET(this->Loading, loading);
    
    cxTimer timer = cxViewAppendTimer(this, 1.0f, 1);
    CX_EVENT_QUICK(timer->onArrive, cxLoadingArrive);
    
    cxViewAppend(engine->window, this);
    return this;
}


