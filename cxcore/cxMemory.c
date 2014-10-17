//
//  cxMemory.c
//  cxCore
//
//  Created by xuhua on 5/16/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxMemory.h"

CX_TYPE(cxMemory, cxObject)
{}
CX_INIT(cxMemory, cxObject)
{}
CX_FREE(cxMemory, cxObject)
{
    allocator->free(this->data);
}
CX_TERM(cxMemory, cxObject)

cxMemory cxMemoryCreate(cxInt size)
{
    cxMemory this = CX_CREATE(cxMemory);
    this->data = allocator->malloc(size);
    this->size = size;
    return this;
}