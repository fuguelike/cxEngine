//
//  cxStack.h
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxStack_h
#define cxCore_cxStack_h

#include "cxArray.h"

CX_C_BEGIN

CX_DEF(cxStack, cxObject)
    cxArray array;
CX_END(cxStack, cxObject)

#define CX_STACK_FOREACH(_s_,_e_) CX_ARRAY_FOREACH(((cxStack)_s_)->array,_e_)

void cxStackClear(cxAny pstack);

void cxStackPush(cxAny pstack,cxAny any);

cxAny cxStackTop(cxAny pstack);

cxInt cxStackLength(cxAny pstack);

void cxStackReplaceTop(cxAny pstack,cxAny any);

void cxStackPop(cxAny pstack);

cxAny cxStackObject(cxAny pstack,cxInt index);

void cxStackRemove(cxAny pstack,cxAny pobj);

CX_C_END

#endif
