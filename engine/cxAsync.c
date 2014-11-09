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
        this->Time += cxEngineGetFrameDelta();
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
CX_METHOD_DEF(cxDriver,Exit,cxBool)
{
    if(cxAsyncGetState(this->Async) != cxAsyncStateSuccess){
        CX_CALL(this, Failed, CX_M(void,cxAny,cxAny),cxActionGetView(this),this->Async);
    }
    return true;
}
CX_METHOD_DEF(cxDriver,Failed,void,cxAny pview,cxAny async)
{
    
}
CX_TYPE(cxDriver, cxAction)
{
    CX_METHOD(cxDriver, Step);
    CX_METHOD(cxDriver, Exit);
    CX_METHOD(cxDriver, Failed);
}
CX_INIT(cxDriver, cxAction)
{
    
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





