//
//  cxAsync.h
//  cxEngineStatic
//
//  Created by xuhua on 11/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAsync_h
#define cxEngine_cxAsync_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

typedef enum {
    cxAsyncStateInit,
    cxAsyncStateFailed,
    cxAsyncStateSuccess,
    cxAsyncStateWait
}cxAsyncState;

CX_DEF(cxAsync, cxObject)
    CX_FIELD_DEF(cxLong Tag);
    CX_FIELD_DEF(cxAsyncState State);
    CX_METHOD_DEF(void, Running,cxAny item);
    CX_FIELD_DEF(cxInt Count);
    CX_FIELD_DEF(cxFloat Time);
    CX_FIELD_DEF(cxAny View);
    CX_METHOD_DEF(void, Init, cxAny);
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

CX_C_END

#endif
