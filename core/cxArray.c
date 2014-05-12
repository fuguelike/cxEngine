//
//  cxArray.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//


#include "cxArray.h"

void __cxArrayInitObject(cxAny object,cxAny json,cxAny hash)
{
    //load items
    CX_OBJECT_INIT_SUPER(cxObject);
}

CX_OBJECT_INIT(cxArray, cxObject)
{
    CX_OBJECT_INIT_OVERRIDE(cxArray);
    utarray_new(this->uta, &ut_ptr_icd);
}
CX_OBJECT_FREE(cxArray, cxObject)
{
    cxArrayClean(this);
    utarray_free(this->uta);
}
CX_OBJECT_TERM(cxArray, cxObject)

void cxArrayClean(cxArray array)
{
    CX_ARRAY_FOREACH(array, e) {
        cxAny any = cxArrayObject(e);
        CX_RELEASE(any);
    }
    utarray_clear(array->uta);
}

void cxArrayFastRemoveAtIndex(cxArray array,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxInt len = cxArrayLength(array);
    //only have 1 element
    if(len == 1){
        cxArrayClean(array);
        return;
    }
    cxAny srcObject = cxArrayAtIndex(array, index);
    CX_ASSERT(srcObject != NULL, "get src object error");
    //move last -> current
    cxInt last = cxArrayLength(array) - 1;
    void *dst = array->uta->d + array->uta->icd.sz * index;
    void *src = array->uta->d + array->uta->icd.sz * last;
    memcpy(dst, src, array->uta->icd.sz);
    array->uta->i--;
    //remove current object
    CX_RELEASE(srcObject);
}

void cxArrayUpdate(cxArray array,cxAny nAny,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxPointer *ele = (cxPointer *)utarray_eltptr(array->uta, index);
    if(ele == NULL){
        return;
    }
    cxAny oAny = cxArrayObject(ele);
    if(oAny == nAny){
        return;
    }
    CX_RELEASE(oAny);
    *ele = nAny;
    CX_RETAIN(nAny);
}

void cxArrayAppends(cxArray array, cxArray data)
{
    CX_ARRAY_FOREACH(data, ele) {
        cxAny obj = cxArrayObject(ele);
        cxArrayAppend(array, obj);
    }
}

void cxArrayAppend(cxArray array, cxAny any)
{
    utarray_push_back(array->uta, &any);
    CX_RETAIN(any);
}

cxIndex cxArrayObjectIndex(cxArray array,cxAny any)
{
    CX_ARRAY_FOREACH(array, e) {
        cxAny tmp = cxArrayObject(e);
        if(tmp == any) {
            return (cxIndex)utarray_eltidx(array->uta,e);
        }
    }
    return CX_INVALID_INDEX;
}

void cxArrayRemove(cxArray array,cxAny any)
{
    cxInt index = cxArrayObjectIndex(array, any);
    if(index != CX_INVALID_INDEX) {
        utarray_erase(array->uta, index, 1);
        CX_RELEASE(any);
    }
}

void cxArrayFastRemove(cxArray array,cxAny any)
{
    cxInt index = cxArrayObjectIndex(array, any);
    if(index != CX_INVALID_INDEX) {
        cxArrayFastRemoveAtIndex(array, index);
    }
}

void cxArrayRemoveAtIndex(cxArray array,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxAny any = cxArrayAtIndex(array, index);
    utarray_erase(array->uta, index, 1);
    CX_RELEASE(any);
}

cxAny cxArrayAtIndex(cxArray array,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxPointer *e = (cxPointer *)utarray_eltptr(array->uta, index);
    return cxArrayObject(e);
}

cxAny cxArrayFirst(cxArray array)
{
    cxPointer *e = (cxPointer *)utarray_front(array->uta);
    return e != NULL ? cxArrayObject(e): NULL;
}

cxAny cxArrayLast(cxArray array)
{
    cxPointer *e = (cxPointer *)utarray_back(array->uta);
    return e != NULL ? cxArrayObject(e): NULL;
}

void cxArrayRemoveFirst(cxArray array)
{
    cxAny any = cxArrayFirst(array);
    if(any != NULL) {
        utarray_erase(array->uta, 0, 1);
        CX_RELEASE(any);
    }
}

void cxArrayRemoveLast(cxArray array)
{
    cxAny any = cxArrayLast(array);
    if(any != NULL) {
        utarray_pop_back(array->uta);
        CX_RELEASE(any);
    }
}



