//
//  cxLoading.h
//  cxEngine
//
//  Created by xuhua on 11/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngineIOS_cxLoading_h
#define cxEngineIOS_cxLoading_h

#include <engine/cxView.h>
#include <engine/cxAsync.h>
#include <actions/cxTimer.h>

CX_C_BEGIN

CX_DEF(cxLoading, cxView)
    CX_FIELD_DEF(cxBool Success);
    cxTimer stepTimer;
    CX_FIELD_DEF(cxInt Index);
    CX_FIELD_DEF(cxInt Step);
    CX_METHOD_DEF(void, onExit, cxAny);
    CX_METHOD_DEF(void, onStep, cxAny);
    CX_METHOD_DEF(void, onStart, cxAny);
    cxArray asyncs;
CX_END(cxLoading, cxView)

CX_FIELD_GET(cxLoading, cxBool, Success);
CX_FIELD_GET(cxLoading, cxInt, Index);
CX_FIELD_GET(cxLoading, cxInt, Step);

CX_INLINE cxFloat cxLoadingGetProgress(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxLoading);
    cxFloat i = this->Index + 1;
    cxFloat a = this->Step;
    return i / a;
}

cxAsync cxLoadingCurrentItem(cxAny pview);

void cxLoadingAppend(cxAny pview,cxAsyncFunc running);

void cxLoadingAppendItem(cxAny pview,cxAny pitem);

#define cxLoadingAppendType(_o_,_t_)  cxLoadingAppendItem(_o_,CX_CREATE(_t_))

void cxLoadingStop(cxAny pview);

void cxLoadingStart(cxAny pview);

void cxLoaingFinished(cxAny pview);

CX_C_END

#endif
