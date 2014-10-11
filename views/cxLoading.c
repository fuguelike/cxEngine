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

static void cxLoadingTimerArrive(cxAny sender)
{
    CX_ASSERT_VALUE(cxActionGetView(sender), cxLoading, this);
    CX_METHOD_RUN(this->onStep, this);
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

void cxLoaingFinished(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxActionStop(this->stepTimer);
}

CX_OBJECT_TYPE(cxLoading, cxView)
{
    
}
CX_OBJECT_INIT(cxLoading, cxView)
{
    this->Step = 1;
    CX_SET(cxView, this, Touch, cxLoadingTouch);
    CX_SET(cxView, this, Key, cxLoadingKey);
}
CX_OBJECT_FREE(cxLoading, cxView)
{

}
CX_OBJECT_TERM(cxLoading, cxView)

void cxLoadingStart(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxEngine engine = cxEngineInstance();
    CX_METHOD_RUN(this->onStart, this);
    this->stepTimer = cxViewAppendTimer(this, 1.0f/30.0f, CX_FOREVER);
    CX_ADD(cxTimer, this->stepTimer, onArrive, cxLoadingTimerArrive);
    CX_ADD(cxAction, this->stepTimer, onExit, cxLoadingTimerExit);
    cxViewAppend(engine->window, this);
}


