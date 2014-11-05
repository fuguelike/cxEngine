//
//  cxMemory.h
//  cxCore
//
//  Created by xuhua on 5/16/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxMemory_h
#define cxCore_cxMemory_h

#include "cxCore.h"

CX_C_BEGIN

CX_DEF(cxMemory, cxObject)
    cxAny data;
    cxInt size;
    cxInt number;
CX_END(cxMemory, cxObject)

cxMemory cxMemoryCreate(cxInt size);

CX_C_END

#endif
