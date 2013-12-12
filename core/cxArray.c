//
//  cxArray.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxArray.h"


CX_OBJECT_INIT(cxArray, cxObject)
{
    utarray_new(this->utArray, &ut_ptr_icd);
}
CX_OBJECT_FREE(cxArray, cxObject)
{
    cxArrayClean(this);
    utarray_free(this->utArray);
}
CX_OBJECT_TERM(cxArray, cxObject)

void cxArrayClean(cxArray array)
{
    CX_ARRAY_FOREACH(array, e) {
        cxAny any = cxArrayObject(e);
        CX_RELEASE(any);
    }
    utarray_clear(array->utArray);
}

void cxArrayUpdate(cxArray array,cxAny nAny,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxPointer *ele = (cxPointer *)utarray_eltptr(array->utArray, index);
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
    utarray_push_back(array->utArray, &any);
    CX_RETAIN(any);
}

cxIndex cxArrayObjectIndex(cxArray array,cxAny any)
{
    CX_ARRAY_FOREACH(array, e) {
        cxAny tmp = cxArrayObject(e);
        if(tmp == any) {
            return (cxIndex)utarray_eltidx(array->utArray,e);
        }
    }
    return CX_INVALID_INDEX;
}

void cxArrayRemove(cxArray array,cxAny any)
{
    int index = cxArrayObjectIndex(array, any);
    if(index != CX_INVALID_INDEX) {
        utarray_erase(array->utArray, index, 1);
        CX_RELEASE(any);
    }
}

void cxArrayRemoveAtIndex(cxArray array,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxAny any = cxArrayAtIndex(array, index);
    utarray_erase(array->utArray, index, 1);
    CX_RELEASE(any);
}

cxAny cxArrayAtIndex(cxArray array,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxPointer *e = (cxPointer *)utarray_eltptr(array->utArray, index);
    return cxArrayObject(e);
}

cxAny cxArrayFirst(cxArray array)
{
    cxPointer *e = (cxPointer *)utarray_front(array->utArray);
    return e != NULL ? cxArrayObject(e): NULL;
}

cxAny cxArrayLast(cxArray array)
{
    cxPointer *e = (cxPointer *)utarray_back(array->utArray);
    return e != NULL ? cxArrayObject(e): NULL;
}

void cxArrayRemoveFirst(cxArray array)
{
    cxAny any = cxArrayFirst(array);
    if(any != NULL) {
        utarray_erase(array->utArray, 0, 1);
        CX_RELEASE(any);
    }
}

void cxArrayRemoveLast(cxArray array)
{
    cxAny any = cxArrayLast(array);
    if(any != NULL) {
        utarray_pop_back(array->utArray);
        CX_RELEASE(any);
    }
}



