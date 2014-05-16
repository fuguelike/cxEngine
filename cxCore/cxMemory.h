//
//  cxMemory.h
//  cxCore
//
//  Created by xuhua on 5/16/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxMemory_h
#define cxCore_cxMemory_h

#include "cxBase.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxMemory, cxObject)
    cxPointer data;
    cxInt size;
CX_OBJECT_END(cxMemory)

cxMemory cxMemoryCreate(cxInt size);

CX_C_END

#endif
