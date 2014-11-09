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
    cxAsyncState state = cxAsyncDrive(this, item);
    //wait
    CX_RETURN(state == cxAsyncStateRunning);
    //success
    if(state == cxAsyncStateSuccess){
        this->Index ++;
        CX_CALL(this, Step, CX_M(void));
    }
    //break init
    if(state == cxAsyncStateFailed){
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
    CX_CALL(this, Exit, CX_M(void));
    cxViewRemove(this);
}

void cxLoadingAppend(cxAny pview,cxAny pitem)
{
    CX_ASSERT_THIS(pview, cxLoading);
    CX_ASSERT_VALUE(pitem, cxAsync, item);
    cxArrayAppend(this->asyncs, item);
    this->Step ++;
}

void cxLoaingFireFinished(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxActionStop(this->stepTimer);
}

CX_METHOD_DEF(cxLoading,Exit,void)
{
    
}
CX_METHOD_DEF(cxLoading,Step,void)
{
    
}
CX_METHOD_DEF(cxLoading,Start,void)
{
    
}
CX_METHOD_DEF(cxLoading,OnTouch, cxBool, const cxTouchItems *points)
{
    return true;
}
CX_METHOD_DEF(cxLoading, OnKey, cxBool, const cxKey *key)
{
    return true;
}
CX_TYPE(cxLoading, cxView)
{
    CX_METHOD(cxLoading, OnTouch);
    CX_METHOD(cxLoading, OnKey);
    CX_METHOD(cxLoading, Exit);
    CX_METHOD(cxLoading, Step);
    CX_METHOD(cxLoading, Start);
}
CX_INIT(cxLoading, cxView)
{
    this->Success = true;
    this->Step = 0;
    this->asyncs = CX_ALLOC(cxArray);
}
CX_FREE(cxLoading, cxView)
{
    CX_RELEASE(this->asyncs);
}
CX_TERM(cxLoading, cxView)

void cxLoadingFireStop(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    CX_RETURN(this->stepTimer == NULL);
    cxActionStop(this->stepTimer);
}

void cxLoadingFireStart(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxEngine engine = cxEngineInstance();
    if(cxArrayLength(this->asyncs) == 0){
        CX_CALL(this, Exit, CX_M(void));
        return;
    }
    //run start and step 1
    CX_CALL(this, Start, CX_M(void));
    CX_CALL(this, Step, CX_M(void));
    //begin timer
    this->stepTimer = cxViewAppendTimer(this, 1.0f/30.0f, CX_FOREVER);
    CX_ADD(cxTimer, this->stepTimer, onArrive, cxLoadingTimerArrive);
    CX_ADD(cxAction, this->stepTimer, onExit, cxLoadingTimerExit);
    cxViewAppend(engine->Window, this);
}


