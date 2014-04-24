//
//  cxArray.h
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxArray_h
#define cxEngine_cxArray_h

#include "utarray.h"
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

void    cxArrayRemoveAtIndex(cxArray array,cxIndex index);

cxAny   cxArrayAtIndex(cxArray array,cxIndex index);

cxAny   cxArrayFirst(cxArray array);

cxAny   cxArrayLast(cxArray array);

void    cxArrayRemoveFirst(cxArray array);

void    cxArrayRemoveLast(cxArray array);

CX_C_END

#endif
