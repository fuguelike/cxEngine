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

typedef cxInt (*cxArrayCmpFunc)(cxConstAny lv,cxConstAny rv);

CX_OBJECT_DEF(cxArray, cxObject)
    UT_array *utArray;
CX_OBJECT_END(cxArray, cxObject)

#define CX_ARRAY_FOREACH(_a_,_e_)                                               \
cxPointer *_e_ = NULL;                                                          \
if((_a_)!=NULL)while((_e_=(cxPointer)utarray_next((_a_)->utArray, _e_))!=NULL)

#define CX_ARRAY_REVERSE(_a_,_e_)                                               \
cxPointer *_e_ = NULL;                                                          \
if((_a_)!=NULL)while((_e_=(cxPointer)utarray_prev((_a_)->utArray, _e_))!=NULL)

#define cxArrayLength(_a_) (((_a_) == NULL) ? 0 : utarray_len((_a_)->utArray))

#define cxArraySort(_a_,_cmp_) if((_a_) != NULL){utarray_sort((_a_)->utArray, _cmp_);}

#define cxArrayObject(_e_)   (*_e_)

void    cxArrayClean(cxArray array);

void    cxArrayUpdate(cxArray array,cxAny any,cxInt index);

void    cxArrayAppends(cxArray array, cxArray data);

void    cxArrayAppend(cxArray array, cxAny any);

//insert index before
void    cxArrayInsert(cxArray array,cxAny any,cxInt index);

cxInt cxArrayIndex(cxArray array,cxAny any);

void    cxArrayRemove(cxArray array,cxAny any);

void    cxArrayFastRemove(cxArray array,cxAny any);

void    cxArrayFastRemoveAtIndex(cxArray array,cxInt index);

void    cxArrayRemoveAtIndex(cxArray array,cxInt index);

cxAny   cxArrayAtIndex(cxArray array,cxInt index);

cxAny   cxArrayFirst(cxArray array);

cxAny   cxArrayLast(cxArray array);

void    cxArrayRemoveFirst(cxArray array);

void    cxArrayRemoveLast(cxArray array);

CX_C_END

#endif
