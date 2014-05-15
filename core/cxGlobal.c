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
#include "cxType.h"

void cxGlobalInit()
{
    cxAllocatorInit();
    cxAutoPoolInit();
    cxTypeInit();
}

void cxGlobalFree()
{
    cxTypeFree();
    cxAutoPoolFree();
    cxAllocatorFree();
}