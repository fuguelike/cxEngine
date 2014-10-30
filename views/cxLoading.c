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

CX_DEF(cxLoadingItem, cxObject)
    cxLoadingFunc func;
CX_END(cxLoadingItem, cxObject)
CX_TYPE(cxLoadingItem, cxObject)
{}
CX_INIT(cxLoadingItem, cxObject)
{}
CX_FREE(cxLoadingItem, cxObject)
{}
CX_TERM(cxLoadingItem, cxObject)

static cxBool cxLoadingTouch(cxAny pview,const cxTouchItems *points)
{
    return true;
}

static cxBool cxLoadingKey(cxAny pview,const cxKey *key)
{
    return true;
}

static void cxLoadingTimerArrive(cxAny sender)
{
    CX_ASSERT_VALUE(cxActionGetView(sender), cxLoading, this);
    CX_METHOD_RUN(this->onStep, this);
    //run step method
    cxLoadingItem item = NULL;
    if(this->Index >= 0 && this->Index < cxArrayLength(this->items)){
        item = cxArrayAtIndex(this->items, this->Index);
    }
    cxBool ret = true;
    if(item != NULL && item->func != NULL){
        ret = item->func(this);
    }
    //break init
    if(!ret){
        this->Success = false;
        cxActionStop(this->stepTimer);
        return;
    }
    //
    this->Index ++;
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

void cxLoadingAppend(cxAny pview,cxLoadingFunc func)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxLoadingItem item = CX_ALLOC(cxLoadingItem);
    item->func = func;
    cxArrayAppend(this->items, item);
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
    this->Step = 1;
    CX_SET(cxView, this, Touch, cxLoadingTouch);
    CX_SET(cxView, this, Key, cxLoadingKey);
    this->items = CX_ALLOC(cxArray);
}
CX_FREE(cxLoading, cxView)
{
    CX_RELEASE(this->items);
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
    CX_METHOD_RUN(this->onStart, this);
    this->stepTimer = cxViewAppendTimer(this, 1.0f/30.0f, CX_FOREVER);
    CX_ADD(cxTimer, this->stepTimer, onArrive, cxLoadingTimerArrive);
    CX_ADD(cxAction, this->stepTimer, onExit, cxLoadingTimerExit);
    cxViewAppend(engine->Window, this);
}


