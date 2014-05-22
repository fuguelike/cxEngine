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
    cxBool isLoading;
    cxBool autoFinished;
    cxAny object;
    CX_EVENT_ALLOC(onFinished);
    CX_EVENT_ALLOC(onLoading);
    CX_EVENT_ALLOC(onStart);
CX_OBJECT_END(cxLoading)

cxAny cxLoadingObject(cxAny pview);

void cxLoadingSetObject(cxAny pview,cxAny object);

void cxLoadingOnUpdate(cxEvent *event);

cxBool cxLoadingTouch(cxAny pview,cxTouch *touch);

void cxLoadingStart(cxLoading this);

void cxLoaingFinished(cxAny pview);

CX_C_END

#endif
