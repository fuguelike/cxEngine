//
//  cxLooper.h
//  cxEngine
//
//  Created by xuhua on 11/10/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLooper_h
#define cxEngine_cxLooper_h

#include <uv/uv.h>
#include <cxcore/cxBase.h>
#include "cxTCP.h"

CX_C_BEGIN

CX_DEF(cxLooper, cxObject)
    uv_loop_t looper;
CX_END(cxLooper, cxObject)

void cxLooperUpdate(cxAny loop);

CX_C_END

#endif
