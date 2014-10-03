//
//  cxMemPool.h
//  cxCore
//  自动释放池
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxMemPool_h
#define cxCore_cxMemPool_h

#include "cxCore.h"
#include "cxArray.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxMemPool, cxObject)
    cxArray objects;
CX_OBJECT_END(cxMemPool, cxObject)

void cxMemPoolInit();

void cxMemPoolFree();

cxAny cxMemPoolAppend(cxAny any);

void cxMemPoolBegin();

void cxMemPoolClear();

void cxMemPoolPush();

void cxMemPoolPop();

CX_C_END

#endif
