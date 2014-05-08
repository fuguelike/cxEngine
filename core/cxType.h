//
//  cxInitType.h
//  cxEngine
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxInitType_h
#define cxEngine_cxInitType_h

#include "cxBase.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxType, cxObject)
    CX_METHOD_ALLOC(cxAny, Create);
    CX_METHOD_ALLOC(cxAny, Alloc);
    cxConstType typeName;
    cxType superType;
CX_OBJECT_END(cxType)

void cxInitTypes();

CX_C_END

#endif








