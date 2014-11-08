//
//  cxAsync.h
//  cxEngineStatic
//  异步操作
//  Created by xuhua on 11/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAsync_h
#define cxEngine_cxAsync_h

#include <cxcore/cxBase.h>
#include "cxAction.h"

CX_C_BEGIN

typedef enum {
    //初始化状态
    cxAsyncStateInit,
    //操作失败状态
    cxAsyncStateFailed,
    //操作成功状态
    cxAsyncStateSuccess,
    //正在执行状态
    cxAsyncStateRunning
}cxAsyncState;

CX_DEF(cxAsync, cxObject)
    CX_FIELD_DEF(cxLong Tag);
    CX_FIELD_DEF(cxAsyncState State);
    CX_FIELD_DEF(cxInt Count);
    CX_FIELD_DEF(cxFloat Time);
    CX_FIELD_DEF(cxAny View);
CX_END(cxAsync, cxObject)

CX_FIELD_IMP(cxAsync, cxLong, Tag);
CX_FIELD_GET(cxAsync, cxInt, Count);
CX_FIELD_GET(cxAsync, cxFloat, Time);
CX_FIELD_IMP(cxAsync, cxAny, View);
CX_FIELD_IMP(cxAsync, cxAsyncState, State);

cxAsyncState cxAsyncDrive(cxAny pview, cxAny pitem);

CX_INLINE void cxAsyncReset(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxAsync);
    cxAsyncSetState(this, cxAsyncStateInit);
    this->Count = 0;
    this->Time = 0;
}

//async驱动器
CX_DEF(cxDriver, cxAction)
    CX_FIELD_DEF(cxAny Async);
CX_END(cxDriver, cxAction)

CX_FIELD_GET(cxDriver, cxAny, Async);

cxDriver cxDriverCreate(cxAny async);

CX_C_END

#endif
