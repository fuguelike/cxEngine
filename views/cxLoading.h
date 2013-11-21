//
//  cxLoading.h
//  cxEngine
//
//  Created by xuhua on 11/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngineIOS_cxLoading_h
#define cxEngineIOS_cxLoading_h

#include <core/cxView.h>

CX_C_BEGIN

typedef void (*cxLoadingFunc)(cxAny object);

CX_OBJECT_DEF(cxLoading, cxView)
    cxBool isLoading;
    CX_METHOD_DEF(cxLoadingFunc, Finished);
    CX_METHOD_DEF(cxLoadingFunc, Loading);
    cxAny object;
CX_OBJECT_END(cxLoading)

cxAny cxLoadingObject(cxAny pview);

void cxLoadingSetObject(cxAny pview,cxAny object);

void cxLoadingOnUpdate(cxEvent *event);

cxBool cxLoadingTouch(cxAny pview,cxTouch *touch);

cxAny cxLoadingStart(cxLoadingFunc loading,cxLoadingFunc finished);

void cxLoaingFinished(cxAny pview);

CX_C_END

#endif
