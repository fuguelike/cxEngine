//
//  cxAutoPool.h
//  cxEngine
//  自动释放池
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAutoPool_h
#define cxEngine_cxAutoPool_h

#include "cxBase.h"
#include "cxArray.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxAutoPool, cxObject)
    cxArray objects;
CX_OBJECT_END(cxAutoPool)

void cxAutoPoolInit();

void cxAutoPoolFree();

cxAny cxAutoPoolAppend(cxAny any);

void cxAutoPoolBegin();

void cxAutoPoolClean();

void cxAutoPoolPush();

void cxAutoPoolPop();

CX_C_END

#endif
