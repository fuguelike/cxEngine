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

static cxBool cxLoadingTouch(cxAny pview,const cxTouchItems *points)
{
    return true;
}

static cxBool cxLoadingKey(cxAny pview,const cxKey *key)
{
    return true;
}

cxAsync cxLoadingCurrentItem(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxAsync item = NULL;
    if(this->Index >= 0 && this->Index < cxArrayLength(this->asyncs)){
        item = cxArrayAtIndex(this->asyncs, this->Index);
    }
    return item;
}

static void cxLoadingTimerArrive(cxAny sender)
{
    CX_ASSERT_VALUE(cxActionGetView(sender), cxLoading, this);
    //run step method
    cxAsync item =  cxLoadingCurrentItem(this);
    CX_ASSERT(item != NULL && item->Running != NULL, "item error");
    cxAsyncState ret = cxAsyncDrive(this, item);
    //wait
    if(ret == cxAsyncStateWait){
        return;
    }
    //success
    if(ret == cxAsyncStateSuccess){
        this->Index ++;
        CX_METHOD_RUN(this->onStep, this);
    }
    //break init
    if(ret == cxAsyncStateFailed){
        this->Success = false;
        cxActionStop(this->stepTimer);
    }
    if(this->Index == this->Step){
        cxActionStop(this->stepTimer);
    }
}

static void cxLoadingTimerExit(cxAny sender)
{
    CX_ASSERT_VALUE(cxActionGetView(sender), cxLoading, this);
    CX_METHOD_RUN(this->onExit, this);
    cxViewRemove(this);
}

void cxLoadingAppendItem(cxAny pview,cxAny pitem)
{
    CX_ASSERT_THIS(pview, cxLoading);
    CX_ASSERT_VALUE(pitem, cxAsync, item);
    CX_ASSERT(item->Running != NULL, "item not set running method");
    cxArrayAppend(this->asyncs, item);
    this->Step ++;
}

void cxLoadingAppend(cxAny pview,cxAsyncFunc running)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxAsync item = CX_ALLOC(cxAsync);
    item->Running = running;
    cxArrayAppend(this->asyncs, item);
    this->Step ++;
    CX_RELEASE(item);
}

void cxLoaingFinished(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxActionStop(this->stepTimer);
}

CX_TYPE(cxLoading, cxView)
{
    
}
CX_INIT(cxLoading, cxView)
{
    this->Success = true;
    this->Step = 0;
    CX_SET(cxView, this, Touch, cxLoadingTouch);
    CX_SET(cxView, this, Key, cxLoadingKey);
    this->asyncs = CX_ALLOC(cxArray);
}
CX_FREE(cxLoading, cxView)
{
    CX_RELEASE(this->asyncs);
}
CX_TERM(cxLoading, cxView)

void cxLoadingStop(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    CX_RETURN(this->stepTimer == NULL);
    cxActionStop(this->stepTimer);
}

void cxLoadingStart(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxEngine engine = cxEngineInstance();
    //run start and step 1
    CX_METHOD_RUN(this->onStart, this);
    CX_METHOD_RUN(this->onStep, this);
    //begin timer
    this->stepTimer = cxViewAppendTimer(this, 1.0f/30.0f, CX_FOREVER);
    CX_ADD(cxTimer, this->stepTimer, onArrive, cxLoadingTimerArrive);
    CX_ADD(cxAction, this->stepTimer, onExit, cxLoadingTimerExit);
    cxViewAppend(engine->Window, this);
}


