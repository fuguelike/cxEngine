//
//  cxAnyArray.c
//  cxCore
//
//  Created by xuhua on 8/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxAnyArray.h"

CX_OBJECT_TYPE(cxAnyArray, cxObject)
{
    
}
CX_OBJECT_INIT(cxAnyArray, cxObject)
{
    
}
CX_OBJECT_FREE(cxAnyArray, cxObject)
{
    utarray_free(this->array);
}
CX_OBJECT_TERM(cxAnyArray, cxObject)

cxAnyArray cxAnyArrayAllocImp(cxInt size)
{
    cxAnyArray this = CX_ALLOC(cxAnyArray);
    this->icd.copy = NULL;
    this->icd.dtor = NULL;
    this->icd.init = NULL;
    this->icd.sz = size;
    utarray_new(this->array, &this->icd);
    return this;
}

cxAnyArray cxAnyArrayCreateImp(cxInt size)
{
    cxAnyArray this = cxAnyArrayAllocImp(size);
    return CX_AUTO(this);
}