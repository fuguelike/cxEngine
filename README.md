cxEngine V2.0

support json object

2D game engine,base openGL ES 2.0

support android ios

Email: cxuhua@gmail.com

QQ:4575407

定义一个类型
------
```
CX_OBJECT_DEF(typeName, baseType)
    //定义字段
    int filed;
CX_OBJECT_END(typeName)
```
实现一个类型
------
```
CX_OBJECT_TYPE(typeName, baseType)
{
    //当类型被注册时调用
}
CX_OBJECT_INIT(typeName, baseType)
{
    //当对象被初始化
}
CX_OBJECT_FREE(typeName, baseType)
{
    //当对象被销毁时
}
CX_OBJECT_TERM(typeName, baseType)
```
创建对象,对象使用引用计数管理
---------------
```
cxAny object1 = CX_CREATE(typeName); //对象将放入自动释放池
cxAny object2 = CX_ALLOC(typeName);//对象不放入自动释放池，需要 CX_RELEASE释放
```
例子
--
cxArray.h
```
//
//  cxArray.h
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxArray_h
#define cxCore_cxArray_h

#include <utarray.h>
#include "cxCore.h"

CX_C_BEGIN

//cxPointer element

typedef int (*cxArrayCmpFunc)(cxPointer *lp,cxPointer *rp);

CX_OBJECT_DEF(cxArray, cxObject)
    UT_array *utArray;
CX_OBJECT_END(cxArray)

#define CX_ARRAY_FOREACH(_a_,_e_)                                                           \
cxPointer *_e_ = NULL;                                                                      \
while((_a_) != NULL && (_e_ = (cxPointer)utarray_next((_a_)->utArray, _e_)) != NULL)

#define CX_ARRAY_REVERSE(_a_,_e_)                                                           \
cxPointer *_e_ = NULL;                                                                      \
while((_a_) != NULL && (_e_ = (cxPointer)utarray_prev((_a_)->utArray, _e_)) != NULL)

#define cxArrayLength(_a_) (((_a_) == NULL) ? 0 : utarray_len((_a_)->utArray))

#define cxArraySort(_a_,_cmp_) if((_a_) != NULL){utarray_sort((_a_)->utArray, _cmp_);}

#define cxArrayObject(_e_)   (*_e_)

void    cxArrayClean(cxArray array);

void    cxArrayUpdate(cxArray array,cxAny any,cxIndex index);

void    cxArrayAppends(cxArray array, cxArray data);

void    cxArrayAppend(cxArray array, cxAny any);

cxIndex cxArrayIndex(cxArray array,cxAny any);

void    cxArrayRemove(cxArray array,cxAny any);

void    cxArrayFastRemove(cxArray array,cxAny any);

void    cxArrayFastRemoveAtIndex(cxArray array,cxIndex index);

void    cxArrayRemoveAtIndex(cxArray array,cxIndex index);

cxAny   cxArrayAtIndex(cxArray array,cxIndex index);

cxAny   cxArrayFirst(cxArray array);

cxAny   cxArrayLast(cxArray array);

void    cxArrayRemoveFirst(cxArray array);

void    cxArrayRemoveLast(cxArray array);

CX_C_END

#endif
```
cxArray.c
```
//
//  cxArray.c
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxArray.h"

CX_OBJECT_TYPE(cxArray, cxObject)
{
    
}
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

void cxArrayFastRemoveAtIndex(cxArray array,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxAny srcObject = cxArrayAtIndex(array, index);
    CX_RELEASE(srcObject);
    cxInt last = cxArrayLength(array) - 1;
    void *dst = array->utArray->d + array->utArray->icd.sz * index;
    void *src = array->utArray->d + array->utArray->icd.sz * last;
    memcpy(dst, src, array->utArray->icd.sz);
    array->utArray->i--;
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
    cxInt index = cxArrayObjectIndex(array, any);
    if(index != CX_INVALID_INDEX) {
        utarray_erase(array->utArray, index, 1);
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




```