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

#define CX_ANY_ARRAY_FOREACH(_o_,_e_,_t_)                                   \
_t_ *_e_ = NULL;                                                            \
if((_o_)!=NULL)while((_e_=(_t_ *)utarray_next((_o_)->array, _e_))!=NULL)


#define CX_ANY_ARRAY_REVERSE(_o_,_e_,_t_)                                   \
_t_ *_e_ = NULL;                                                            \
if((_o_)!=NULL)while((_e_=(_t_ *)utarray_prev((_o_)->array, _e_))!=NULL)

CX_OBJECT_DEF(cxAnyArray, cxObject)
    UT_array *array;
    UT_icd icd;
CX_OBJECT_END(cxAnyArray, cxObject)

cxAnyArray cxAnyArrayAllocImp(cxInt size);

cxAnyArray cxAnyArrayCreateImp(cxInt size);

#define cxAnyArrayAlloc(_t_)          cxAnyArrayAllocImp(sizeof(_t_))

#define cxAnyArrayCreate(_t_)         cxAnyArrayCreateImp(sizeof(_t_))

//_t_ *
#define cxAnyArrayPtr(_o_,_t_)        (_t_ *)(utarray_front((_o_)->array))

#define cxAnyArrayLength(_o_)         (utarray_len((_o_)->array))

//_t_ *
#define cxAnyArrayFirst(_o_,_t_)      cxAnyArrayPtr(_o_,_t_)

//_t_ *
#define cxAnyArrayLast(_o_,_t_)       (_t_ *)utarray_back((_o_)->array)

//_t_ *
#define cxAnyArrayAt(_o_,_i_,_t_)     (_t_ *)utarray_eltptr((_o_)->array, _i_)

#define cxAnyArrayAppend(_o_,_p_)     utarray_push_back((_o_)->array, _p_)

//insert index before
#define cxAnyArrayInsert(_o_,_p_,_i_) utarray_insert((_o_)->array,_p_,_i_)

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
