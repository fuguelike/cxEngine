//
//  cxArray.c
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxType.h"
#include "cxArray.h"

/*
 items:[value1,value2]
*/

CX_SETTER_DEF(cxArray, items)
{
    CX_ASSERT(cxJsonIsArray(value), "items must is array");
    cxJson items = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(items, v)
    cxAny any = cxJsonTocxObject(v);
    if(any != NULL){
        cxArrayAppend(this, any);
    }
    CX_JSON_ARRAY_EACH_END(items, v)
}

CX_OBJECT_TYPE(cxArray, cxObject)
{
    CX_PROPERTY_SETTER(cxArray, items);
}
CX_OBJECT_INIT(cxArray, cxObject)
{
    utarray_new(this->utArray, &ut_ptr_icd);
}
CX_OBJECT_FREE(cxArray, cxObject)
{
    cxArrayClear(this);
    utarray_free(this->utArray);
}
CX_OBJECT_TERM(cxArray, cxObject)

void cxArrayClear(cxAny array)
{
    CX_ASSERT_THIS(array, cxArray);
    CX_ARRAY_FOREACH(this, e) {
        cxAny any = cxArrayObject(e);
        CX_RELEASE(any);
    }
    utarray_clear(this->utArray);
}

void cxArrayUpdate(cxAny array,cxAny nAny,cxInt index)
{
    CX_ASSERT_THIS(array, cxArray);
    CX_ASSERT(index >= 0 && index < cxArrayLength(this), "index invalid");
    cxAny *ele = (cxAny *)utarray_eltptr(this->utArray, index);
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

void cxArrayAppends(cxAny array, cxArray data)
{
    CX_ASSERT_THIS(array, cxArray);
    CX_ARRAY_FOREACH(data, ele) {
        cxAny obj = cxArrayObject(ele);
        cxArrayAppend(this, obj);
    }
}

void cxArrayInsert(cxAny array,cxAny any,cxInt index)
{
    CX_ASSERT_THIS(array, cxArray);
    utarray_insert(this->utArray, &any, index);
    CX_RETAIN(any);
}

void cxArrayAppend(cxAny array, cxAny any)
{
    CX_ASSERT_THIS(array, cxArray);
    utarray_push_back(this->utArray, &any);
    CX_RETAIN(any);
}

cxInt cxArrayObjectIndex(cxAny array,cxAny any)
{
    CX_ASSERT_THIS(array, cxArray);
    CX_ARRAY_FOREACH(this, e) {
        cxAny tmp = cxArrayObject(e);
        if(tmp == any) {
            return (cxInt)utarray_eltidx(this->utArray,e);
        }
    }
    return -1;
}

void cxArrayRemove(cxAny array,cxAny any)
{
    CX_ASSERT_THIS(array, cxArray);
    cxInt index = cxArrayObjectIndex(array, any);
    if(index != -1) {
        utarray_erase(this->utArray, index, 1);
        CX_RELEASE(any);
    }
}

void cxArrayFastRemove(cxAny array,cxInt index)
{
    CX_ASSERT_THIS(array, cxArray);
    CX_ASSERT(index >= 0 && index < cxArrayLength(this), "index invalid");
    cxAny srcObject = cxArrayAtIndex(array, index);
    CX_RELEASE(srcObject);
    cxInt last = cxArrayLength(this) - 1;
    void *dst = this->utArray->d + this->utArray->icd.sz * index;
    void *src = this->utArray->d + this->utArray->icd.sz * last;
    memcpy(dst, src, this->utArray->icd.sz);
    this->utArray->i--;
}

void cxArrayRemoveAtIndex(cxAny array,cxInt index)
{
    CX_ASSERT_THIS(array, cxArray);
    CX_ASSERT(index >= 0 && index < cxArrayLength(this), "index invalid");
    cxAny any = cxArrayAtIndex(this, index);
    utarray_erase(this->utArray, index, 1);
    CX_RELEASE(any);
}

cxAny cxArrayAtIndex(cxAny array,cxInt index)
{
    CX_ASSERT_THIS(array, cxArray);
    CX_ASSERT(index >= 0 && index < cxArrayLength(this), "index invalid");
    cxAny *e = (cxAny *)utarray_eltptr(this->utArray, index);
    return cxArrayObject(e);
}

cxAny cxArrayFirst(cxAny array)
{
    CX_ASSERT_THIS(array, cxArray);
    cxAny *e = (cxAny *)utarray_front(this->utArray);
    return e != NULL ? cxArrayObject(e): NULL;
}

cxAny cxArrayLast(cxAny array)
{
    CX_ASSERT_THIS(array, cxArray);
    cxAny *e = (cxAny *)utarray_back(this->utArray);
    return e != NULL ? cxArrayObject(e): NULL;
}

void cxArrayRemoveFirst(cxAny array)
{
    CX_ASSERT_THIS(array, cxArray);
    cxAny any = cxArrayFirst(this);
    if(any != NULL) {
        utarray_erase(this->utArray, 0, 1);
        CX_RELEASE(any);
    }
}

void cxArrayRemoveLast(cxAny array)
{
    CX_ASSERT_THIS(array, cxArray);
    cxAny any = cxArrayLast(this);
    if(any != NULL) {
        utarray_pop_back(this->utArray);
        CX_RELEASE(any);
    }
}



