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

CX_C_BEGIN

typedef void (*cxLoadingFunc)(cxAny object);

CX_OBJECT_DEF(cxLoading, cxView)
    cxTimer stepTimer;
    CX_FIELD_DEF(cxInt Index);
    CX_FIELD_DEF(cxInt Step);
    CX_METHOD_DEF(void, onExit, cxAny);
    CX_METHOD_DEF(void, onStep, cxAny);
    CX_METHOD_DEF(void, onStart, cxAny);
CX_OBJECT_END(cxLoading, cxView)

CX_FIELD_GET(cxLoading, cxInt, Index);
CX_FIELD_SET(cxLoading, cxInt, Step);

CX_INLINE cxFloat cxLoadingGetProgress(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxLoading);
    cxFloat i = this->Index + 1;
    cxFloat a = this->Step;
    return i / a;
}

void cxLoadingStart(cxAny pview);

void cxLoaingFinished(cxAny pview);

CX_C_END

#endif
