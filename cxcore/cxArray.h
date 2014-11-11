//
//  cxArray.h
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxArray_h
#define cxCore_cxArray_h

#include "utarray.h"
#include "cxCore.h"

CX_C_BEGIN

//cxAny element

typedef cxInt (*cxCmpFunc)(cxConstAny lv,cxConstAny rv);

CX_DEF(cxArray, cxObject)
    UT_array *utArray;
CX_END(cxArray, cxObject)

#define CX_ARRAY_FOREACH(_a_,_e_)                                               \
cxAny *_e_ = NULL;                                                              \
if((_a_)!=NULL)while((_e_=(cxAny)utarray_next((_a_)->utArray, _e_))!=NULL)

#define CX_ARRAY_REVERSE(_a_,_e_)                                               \
cxAny *_e_ = NULL;                                                              \
if((_a_)!=NULL)while((_e_=(cxAny)utarray_prev((_a_)->utArray, _e_))!=NULL)

#define cxArrayLength(_a_) (((_a_) == NULL) ? 0 : utarray_len((_a_)->utArray))

#define cxArraySort(_a_,_cmp_) if((_a_) != NULL){utarray_sort((_a_)->utArray, _cmp_);}

#define cxArrayObject(_e_)   (*_e_)

void    cxArrayClear(cxAny array);

void    cxArrayUpdate(cxAny array,cxAny any,cxInt index);

void    cxArrayAppends(cxAny array, cxArray data);

void    cxArrayAppend(cxAny array, cxAny any);

//insert index before
void    cxArrayInsert(cxAny array,cxAny any,cxInt index);

cxInt cxArrayIndex(cxAny array,cxAny any);

void    cxArrayRemove(cxAny array,cxAny any);

void    cxArrayFastRemove(cxAny array,cxInt index);

void    cxArrayRemoveAtIndex(cxAny array,cxInt index);

cxAny   cxArrayAtIndex(cxAny array,cxInt index);

cxAny   cxArrayFirst(cxAny array);

cxAny   cxArrayLast(cxAny array);

void    cxArrayRemoveFirst(cxAny array);

void    cxArrayRemoveLast(cxAny array);

CX_C_END

#endif
