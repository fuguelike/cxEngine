//
//  cxAsync.c
//  cxEngineStatic
//
//  Created by xuhua on 11/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxAsync.h"

CX_TYPE(cxAsync, cxObject)
{
    //
}
CX_INIT(cxAsync, cxObject)
{
    this->State = cxAsyncStateInit;
}
CX_FREE(cxAsync, cxObject)
{
    
}
CX_TERM(cxAsync, cxObject)

cxAsyncState cxAsyncDrive(cxAny pview, cxAny pitem)
{
    CX_ASSERT_THIS(pitem, cxAsync);
    cxAsyncSetView(this, pview);
    if(this->State == cxAsyncStateSuccess || this->State == cxAsyncStateFailed){
        return this->State;
    }else if(this->State == cxAsyncStateInit){
        cxBool ret = CX_METHOD_GET(false, this->Init, this);
        this->State = ret ? cxAsyncStateRunning : cxAsyncStateFailed;
    }else{
        this->Count ++;
        this->Time += cxEngineGetFrameDelta();
        CX_METHOD_RUN(this->Running,this);
    }
    return this->State;
}

static void cxDriverStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxDriver);
    CX_ASSERT_TYPE(this->Async, cxAsync);
    cxAsyncState state = cxAsyncDrive(cxActionGetView(this), this->Async);
    if(state == cxAsyncStateFailed || state == cxAsyncStateSuccess){
        cxActionStop(this);
    }
}

static void cxDriverOnExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxDriver);
    if(cxAsyncGetState(this->Async) != cxAsyncStateSuccess){
        CX_METHOD_RUN(this->onFailed,cxActionGetView(this),this->Async);
    }
}

CX_TYPE(cxDriver, cxAction)
{
    //
}
CX_INIT(cxDriver, cxAction)
{
    CX_SET(cxAction, this, Step, cxDriverStep);
    CX_ADD(cxAction, this, onExit, cxDriverOnExit);
}
CX_FREE(cxDriver, cxAction)
{
    CX_RELEASE(this->Async);
}
CX_TERM(cxDriver, cxAction)

cxDriver cxDriverCreate(cxAny async)
{
    CX_ASSERT_TYPE(async, cxAsync);
    cxDriver this = CX_CREATE(cxDriver);
    CX_RETAIN_SWAP(this->Async, async);
    cxActionSetTime(this, 15);
    return this;
}





