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