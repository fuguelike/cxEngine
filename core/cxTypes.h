//
//  cxTypes.h
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTypes_h
#define cxEngine_cxTypes_h

#include <utarray.h>
#include "cxBase.h"

CX_C_BEGIN

typedef enum {
    cxTypesString,
    cxTypesDB,
    cxTypesHash,
    cxTypesArray,
    cxTypesNumber,
}cxTypesType;

#define cxTypesIsType(o,t)  ((o) != NULL && ((cxTypes)(o))->type == t)

CX_OBJECT_DEF(cxTypes, cxObject)
    cxTypesType type;
    cxAny any;
CX_OBJECT_END(cxTypes)

cxTypes cxDBTypesCreate(cxAny db);

cxTypes cxHashTypesCreate();

cxTypes cxNumberTypesCreate();

cxTypes cxArrayTypesCreate();

cxTypes cxStringTypesCreate();

cxVec2f cxVec2fValue(cxVec2fRange rv);

CX_C_END

#endif














