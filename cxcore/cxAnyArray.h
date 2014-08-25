//
//  cxAnyArray.h
//  cxCore
//
//  Created by xuhua on 8/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxAnyArray_h
#define cxCore_cxAnyArray_h

#include <utarray.h>
#include "cxBase.h"

CX_C_BEGIN

typedef int (*cxAnyArrayCmpFunc)(cxConstAny lv,cxConstAny rv);

CX_OBJECT_DEF(cxAnyArray, cxObject)
    UT_array *array;
    UT_icd icd;
CX_OBJECT_END(cxAnyArray, cxObject)

cxAnyArray cxAnyArrayAllocImp(cxInt size);

cxAnyArray cxAnyArrayCreateImp(cxInt size);

#define cxAnyArrayAlloc(_t_)          cxAnyArrayAllocImp(sizeof(_t_))

#define cxAnyArrayCreate(_t_)         cxAnyArrayCreateImp(sizeof(_t_))

#define cxAnyArrayPtr(_o_,_t_)        ((_t_ *)(utarray_front((_o_)->array)))

#define cxAnyArrayLength(_o_)         (utarray_len((_o_)->array))

#define cxAnyArrayFirst(_o_,_t_)      (*(_t_ *)utarray_front((_o_)->array))

#define cxAnyArrayLast(_o_,_t_)       (*(_t_ *)utarray_back((_o_)->array))

#define cxAnyArrayAt(_o_,_i_,_t_)     (*(_t_ *)utarray_eltptr((_o_)->array, _i_))

#define cxAnyArrayAppend(_o_,_p_)     utarray_push_back((_o_)->array, &(_p_))

#define cxAnyArrayClean(_o_)          utarray_clear((_o_)->array)

#define cxAnyArrayPush(_o_,_p_)       cxAnyArrayAppend(_o_,_p_)

#define cxAnyArrayPop(_o_)            utarray_pop_back((_o_)->array)

/*
 cxInt cmp(cxConstAny lv,cxConstAny rv)
 {
    cxVec2i l = *(cxVec2i *)lv;
    cxVec2i r = *(cxVec2i *)rv;
    return 0;
 }
*/
#define cxAnyArraySort(_o_,_cmp_) if((_o_) != NULL){utarray_sort((_o_)->array, _cmp_);}

CX_C_END

#endif
