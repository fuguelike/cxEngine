//
//  cxGlobal.c
//  cxEngine
//
//  Created by xuhua on 3/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxGlobal.h"
#include "cxAllocator.h"
#include "cxAutoPool.h"


void cxGlobalInit()
{
    cxAllocatorInit();
    cxAutoPoolInit();
    CX_LOGGER("global init");
}

void cxGlobalFree()
{
    CX_LOGGER("global free");
    cxAutoPoolFree();
    cxAllocatorFree();
}