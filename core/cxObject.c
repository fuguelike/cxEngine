//
//  cxObject.c
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxObject.h"

void __cxObjectAutoInit(cxObject this)
{
    this->cxBase = cxBaseTypeObject;
}

void __cxObjectAutoFree(cxObject this)
{
   
}

cxBool cxObjectIsBaseType(cxAny pobj,cxBaseType type)
{
    cxObject this = pobj;
    CX_RETURN(this == NULL, false);
    return this->cxBase == type;
}

cxConstType cxObjectType(cxAny pobj)
{
    cxObject this = pobj;
    CX_RETURN(this == NULL, NULL);
    return this->cxType;
}

cxBool cxObjectIsType(cxAny pobj,cxConstType type)
{
    cxObject this = pobj;
    CX_RETURN(this == NULL, false);
    return this->cxType == type;
}

void cxObjectSetTag(cxAny obj,cxInt tag)
{
    cxObject this = obj;
    this->cxTag = tag;
}

cxInt cxObjectTag(cxAny obj)
{
    cxObject this = obj;
    return this->cxTag;
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



