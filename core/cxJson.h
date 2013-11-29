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

cxInt cxJsonInt(cxJson json,cxConstChars key);

cxConstChars cxJsonString(cxJson json,cxConstChars key);

cxDouble cxJsonDouble(cxJson json,cxConstChars key);

cxLong cxJsonLong(cxJson json,cxConstChars key);

cxJson cxJsonArray(cxJson json,cxConstChars key);

cxJson cxJsonObject(cxJson json,cxConstChars key);

cxInt cxJsonIntAt(cxJson json,cxInt idx);

cxConstChars cxJsonStringAt(cxJson json,cxInt idx);

cxDouble cxJsonDoubleAt(cxJson json,cxInt idx);

cxLong cxJsonLongAt(cxJson json,cxInt idx);

cxJson cxJsonArrayAt(cxJson json,cxInt idx);

cxJson cxJsonObjectAt(cxJson json,cxInt idx);

cxInt cxJsonArrayLength(cxJson json);

CX_C_END

#endif
