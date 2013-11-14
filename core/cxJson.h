//
//  cxJson.h
//  cxEngine
//
//  Created by xuhua on 11/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxJson_h
#define cxEngine_cxJson_h

#include <jansson.h>
#include "cxBase.h"
#include "cxString.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxJson, cxObject)
    json_t *json;
CX_OBJECT_END(cxJson)

#define CX_JSON_PTR(j)  ((j)->json)

cxJson cxJsonCreate(cxString json);

CX_C_END

#endif
