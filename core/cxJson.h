//
//  cxJson.h
//  cxEngine
//
//  Created by xuhua on 11/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxJson_h
#define cxEngine_cxJson_h

#include <jansson/jansson.h>
#include "cxBase.h"
#include "cxString.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxJson, cxObject)
    json_t *json;
CX_OBJECT_END(cxJson)

#define CX_JSON_PTR(j)  ((j)->json)

#define CX_JSON_ARRAY_EACH_BEG(_j_,_v_)                         \
if((_j_) != NULL){                                              \
    json_t *_value_ = NULL;                                     \
    cxInt _index_ = 0;                                          \
    json_array_foreach(CX_JSON_PTR(_j_), _index_, _value_){     \
        cxJson _v_ = cxJsonAttachAlloc(_value_);

#define CX_JSON_ARRAY_EACH_END(_j_,_v_)                         \
        CX_RELEASE(_v_);                                        \
    }                                                           \
}

#define CX_JSON_OBJECT_EACH_BEG(_j_,_v_)                        \
if((_j_) != NULL){                                              \
    json_t *_value_ = NULL;                                     \
    cxConstChars _key_ = NULL;                                  \
    json_object_foreach(CX_JSON_PTR(_j_), _key_, _value_){      \
        cxJson _v_ = cxJsonAttachAlloc(_value_);

#define CX_JSON_OBJECT_EACH_END(_j_,_v_)                        \
        CX_RELEASE(_v_);                                        \
    }                                                           \
}

cxJson cxJsonCreate(cxString json);

cxJson cxJsonAttachAlloc(json_t *json);

cxInt cxJsonToInt(cxJson json,cxInt dv);

cxConstChars cxJsonToConstChars(cxJson json);

cxString cxJsonToString(cxJson json);

cxDouble cxJsonToDouble(cxJson json,cxDouble dv);

cxLong cxJsonToLong(cxJson json,cxLong dv);

cxBool cxJsonToBool(cxJson json,cxBool dv);

cxInt cxJsonInt(cxJson json,cxConstChars key,cxInt dv);

cxConstChars cxJsonConstChars(cxJson json,cxConstChars key);

cxString cxJsonString(cxJson json,cxConstChars key);

cxDouble cxJsonDouble(cxJson json,cxConstChars key,cxDouble ev);

cxLong cxJsonLong(cxJson json,cxConstChars key,cxLong dv);

cxBool cxJsonBool(cxJson json,cxConstChars key,cxBool dv);

cxJson cxJsonArray(cxJson json,cxConstChars key);

cxJson cxJsonObject(cxJson json,cxConstChars key);

cxInt cxJsonIntAt(cxJson json,cxInt idx,cxInt dv);

cxConstChars cxJsonConstCharsAt(cxJson json,cxInt idx);

cxString cxJsonStringAt(cxJson json,cxInt idx);

cxDouble cxJsonDoubleAt(cxJson json,cxInt idx,cxDouble dv);

cxBool cxJsonBoolAt(cxJson json,cxInt idx,cxBool dv);

cxLong cxJsonLongAt(cxJson json,cxInt idx,cxLong dv);

cxJson cxJsonArrayAt(cxJson json,cxInt idx);

cxJson cxJsonObjectAt(cxJson json,cxInt idx);

cxInt cxJsonArrayLength(cxJson json);

CX_C_END

#endif
