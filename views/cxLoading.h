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
#include <actions/cxTimer.h>

CX_C_BEGIN

typedef enum {
    cxLoadingResultFailed = 0,
    cxLoadingResultSuccess,
    cxLoadingResultWait
}cxLoadingResult;

typedef cxLoadingResult (*cxLoadingFunc)(cxAny object,cxAny item);

CX_DEF(cxLoadingItem, cxObject)
    cxLoadingFunc Running;
CX_END(cxLoadingItem, cxObject)

CX_DEF(cxLoading, cxView)
    CX_FIELD_DEF(cxBool Success);
    cxTimer stepTimer;
    CX_FIELD_DEF(cxInt Count);
    CX_FIELD_DEF(cxFloat Time);
    CX_FIELD_DEF(cxInt Index);
    CX_FIELD_DEF(cxInt Step);
    CX_METHOD_DEF(void, onExit, cxAny);
    CX_METHOD_DEF(void, onStep, cxAny);
    CX_METHOD_DEF(void, onStart, cxAny);
    cxArray items;
CX_END(cxLoading, cxView)

CX_FIELD_GET(cxLoading, cxInt, Count);
CX_FIELD_GET(cxLoading, cxFloat, Time);
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

cxLoadingItem cxLoadingCurrentItem(cxAny pview);

void cxLoadingAppend(cxAny pview,cxLoadingFunc running);

void cxLoadingAppendItem(cxAny pview,cxAny pitem);

#define cxLoadingAppendType(_o_,_t_)  cxLoadingAppendItem(_o_,CX_CREATE(_t_))

void cxLoadingStop(cxAny pview);

void cxLoadingStart(cxAny pview);

void cxLoaingFinished(cxAny pview);

CX_C_END

#endif
