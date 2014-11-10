//
//  cxAsync.c
//  cxEngineStatic
//
//  Created by xuhua on 11/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxAsync.h"

CX_METHOD_DEF(cxAsync, Running,void)
{
    
}
CX_METHOD_DEF(cxAsync, Init, cxBool)
{
    return false;
}
CX_TYPE(cxAsync, cxObject)
{
    CX_METHOD(cxAsync, Running);
    CX_METHOD(cxAsync, Init);
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
        cxBool ret = CX_CALL(this, Init, CX_M(cxBool));
        this->State = ret ? cxAsyncStateRunning : cxAsyncStateFailed;
    }else{
        this->Count ++;
        this->Time += cxEngineGetDelta();
        CX_CALL(this, Running, CX_M(void));
    }
    return this->State;
}

CX_METHOD_DEF(cxDriver,Step,void,cxFloat dt,cxFloat time)
{
    CX_ASSERT_TYPE(this->Async, cxAsync);
    cxAsyncState state = cxAsyncDrive(cxActionGetView(this), this->Async);
    if(state == cxAsyncStateFailed || state == cxAsyncStateSuccess){
        cxActionStop(this);
    }
}
static void cxDriverOnExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxDriver);
    cxAsyncState state = cxAsyncGetState(this->Async);
    cxView view = cxActionGetView(this);
    CX_ASSERT_TYPE(view, cxView);
    if(state != cxAsyncStateSuccess && CX_METHOD_HAS(view, AsyncError)){
        CX_CALL(view, AsyncError, CX_M(void,cxAsync),this->Async);
    }
}
CX_TYPE(cxDriver, cxAction)
{
    CX_METHOD(cxDriver, Step);
}
CX_INIT(cxDriver, cxAction)
{
    CX_ADD(cxAction, this, onExit, cxDriverOnExit);
}
CX_FREE(cxDriver, cxAction)
{
    CX_RELEASE(this->Async);
}
CX_TERM(cxDriver, cxAction)

cxDriver cxDriverCreateImp(cxAny async)
{
    CX_ASSERT_TYPE(async, cxAsync);
    cxDriver this = CX_CREATE(cxDriver);
    CX_RETAIN_SWAP(this->Async, async);
    cxActionSetTime(this, 15);
    return this;
}





