//
//  cxArray.h
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxArray_h
#define cxEngine_cxArray_h

#include <utarray.h>
#include "cxBase.h"

CX_C_BEGIN

//cxPointer element

typedef int (*cxArrayCmpFunc)(cxPointer *lp,cxPointer *rp);

CX_OBJECT_DEF(cxArray, cxObject)
    UT_array *utArray;
CX_OBJECT_END(cxArray)

#define CX_ARRAY_FOREACH(a,e) \
    cxPointer *e = NULL; \
    while((a) != NULL && (e = (cxPointer)utarray_next((a)->utArray, e)) != NULL)

#define CX_ARRAY_REVERSE(a,e) \
    cxPointer *e = NULL; \
    while((a) != NULL && (e = (cxPointer)utarray_prev((a)->utArray, e)) != NULL)

#define cxArrayLength(a) (((a) == NULL) ? 0 : utarray_len((a)->utArray))

#define cxArraySort(a,cmp) if((a) != NULL)utarray_sort((a)->utArray, cmp)

#define cxArrayObject(e)   (*e)

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

CX_SCRIPT_DEF(cxArray);

CX_C_END

#endif
