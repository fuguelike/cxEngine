//
//  cxAnyArray.c
//  cxCore
//
//  Created by xuhua on 8/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxBase.h"
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

void cxAnyArrayAppends(cxAnyArray arr,cxAnyArray dst)
{
    CX_ASSERT_THIS(arr, cxAnyArray);
    if(dst == NULL){
        return;
    }
    CX_ASSERT(this->icd.sz == dst->icd.sz, "size not equ,can't append");
    cxAny ptr = NULL;
    while ((ptr = utarray_next(dst->array, ptr)) != NULL) {
        utarray_push_back(this->array, ptr);
    }
}

cxAnyArray cxAnyArrayClone(cxAnyArray arr)
{
    CX_ASSERT_THIS(arr, cxAnyArray);
    cxAnyArray ret = cxAnyArrayCreateImp((cxInt)this->icd.sz);
    cxInt num = cxAnyArrayLength(this);
    if(num > 0){
        utarray_resize(ret->array, num);
        ret->array->i = num;
        memcpy(ret->array->d, this->array->d, num * this->icd.sz);
    }
    return ret;
}

cxAnyArray cxAnyArrayCreateImp(cxInt size)
{
    cxAnyArray this = cxAnyArrayAllocImp(size);
    return CX_AUTO(this);
}