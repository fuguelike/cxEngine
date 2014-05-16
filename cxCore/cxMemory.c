//
//  cxMemory.c
//  cxCore
//
//  Created by xuhua on 5/16/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxMemory.h"

CX_OBJECT_TYPE(cxMemory, cxObject)
{
    
}
CX_OBJECT_INIT(cxMemory, cxObject)
{
    
}
CX_OBJECT_FREE(cxMemory, cxObject)
{
    allocator->free(this->data);
}
CX_OBJECT_TERM(cxMemory, cxObject)

cxMemory cxMemoryCreate(cxInt size)
{
    cxMemory this = CX_CREATE(cxMemory);
    this->data = allocator->malloc(size);
    this->size = size;
    return this;
}