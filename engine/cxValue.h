//
//  cxValue.h
//  cxCore
//
//  Created by xuhua on 10/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxValue_h
#define cxEngine_cxValue_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

#define CX_VALUE_MAX_SIZE 16

CX_OBJECT_DEF(cxValue, cxObject)
    cxBool isChanged;
    cxInt size;
    cxByte oldVar[CX_VALUE_MAX_SIZE];
    cxByte newVar[CX_VALUE_MAX_SIZE];
    CX_METHOD_DEF(cxBool, ValueEqu, cxAny);
    CX_EVENT_ALLOC(onChanged);
    CX_SLOT_ALLOC(onUpdate);
    CX_FIELD_DEF(cxAny Data);
CX_OBJECT_END(cxValue, cxObject)

cxBool cxValueBinaryEqu(cxAny pobj);

cxBool cxValueFloatEqu(cxAny pobj);

cxBool cxValueVec2fEqu(cxAny pobj);

cxBool cxValueSize2fEqu(cxAny pobj);

CX_FIELD_IMP(cxValue, cxAny, Data);

cxValue cxValueAllocImp(cxInt size);

cxValue cxValueCreateImp(cxInt size);

void cxValueSet(cxAny pthis,cxAny v);

#define cxValueAlloc(_t_)           cxValueAllocImp(sizeof(_t_))

#define cxValueCreate(_t_)          cxValueCreateImp(sizeof(_t_))
//_t_ *
#define cxValueGet(_o_,_t_)         (_t_ *)(_o_)->oldVar

CX_C_END

#endif
