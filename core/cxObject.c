//
//  cxObject.c
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxObject.h"
#include "cxUtil.h"
#include "cxJson.h"

void __cxObjectInitObject(cxAny object,cxAny json)
{
    
}

void __cxObjectAutoInit(cxObject this)
{
    CX_OBJECT_OVERRIDE(cxObject, this);
}

void __cxObjectAutoFree(cxObject this)
{
    CX_METHOD_RELEASE(this->InitObject);
}

//auto release memory block
CX_OBJECT_INIT(cxMemory, cxObject)
{
    
}
CX_OBJECT_FREE(cxMemory, cxObject)
{
    allocator->free(this->pointer);
}
CX_OBJECT_TERM(cxMemory, cxObject)

cxMemory cxMemoryCreate(cxInt size)
{
    cxMemory this = CX_CREATE(cxMemory);
    this->pointer = allocator->malloc(size);
    return this;
}



