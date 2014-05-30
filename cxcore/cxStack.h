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

CX_OBJECT_DEF(cxStack, cxObject)
    cxArray array;
CX_OBJECT_END(cxStack, cxObject)

void    cxStackPush(cxStack stack,cxAny any);

cxAny   cxStackTop(cxStack stack);

cxInt   cxStackLength(cxStack stack);

void    cxStackReplaceTop(cxStack stack,cxAny any);

void    cxStackPop(cxStack stack);

cxAny   cxStackObject(cxStack stack,cxInt index);

CX_C_END

#endif
