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
#include <cxcore/cxBase.h>
#include "cxMath.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxJson, cxObject)
    json_t *json;
CX_OBJECT_END(cxJson)

#define CX_JSON_PTR(j)  ((j)->json)

#define CX_JSON_ARRAY_EACH_BEG(_j_,_v_)                         \
if((_j_) != NULL){                                              \
    json_t *_value_ = NULL;                                     \
    cxInt index = 0;                                            \
    json_array_foreach(CX_JSON_PTR(_j_), index, _value_){       \
        cxJson _v_ = cxJsonAttachAlloc(_value_);

#define CX_JSON_ARRAY_EACH_END(_j_,_v_)                         \
        CX_RELEASE(_v_);                                        \
    }                                                           \
}

#define CX_JSON_OBJECT_EACH_BEG(_j_,_v_)                        \
if((_j_) != NULL){                                              \
    json_t *_value_ = NULL;                                     \
    cxConstChars key = NULL;                                    \
    json_object_foreach(CX_JSON_PTR(_j_), key, _value_){        \
        cxJson _v_ = cxJsonAttachAlloc(_value_);

#define CX_JSON_OBJECT_EACH_END(_j_,_v_)                        \
        CX_RELEASE(_v_);                                        \
    }                                                           \
}

void cxJsonRegisterDouble(cxConstChars key,cxDouble value);

void cxJsonRegisterInt(cxConstChars key,cxInt value);

void cxJsonRegisterLong(cxConstChars key,cxLong value);

void cxJsonInit();

void cxJsonFree();

cxJson cxJsonCreate(cxString json);

cxBool cxJsonIsInt(cxJson json);

cxBool cxJsonIsDouble(cxJson json);

cxBool cxJsonIsNumber(cxJson json);

cxBool cxJsonIsString(cxJson json);

cxBool cxJsonIsBool(cxJson json);

cxBool cxJsonIsArray(cxJson json);

cxBool cxJsonIsObject(cxJson json);

cxJson cxJsonAttachAlloc(json_t *json);

cxJson cxJsonToArray(cxJson json);

cxJson cxJsonToObject(cxJson json);

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

cxBool cxJsonExists(cxJson json,cxConstChars key);

cxInt cxJsonIntAt(cxJson json,cxInt idx,cxInt dv);

cxConstChars cxJsonConstCharsAt(cxJson json,cxInt idx);

cxString cxJsonStringAt(cxJson json,cxInt idx);

cxDouble cxJsonDoubleAt(cxJson json,cxInt idx,cxDouble dv);

cxBool cxJsonBoolAt(cxJson json,cxInt idx,cxBool dv);

cxLong cxJsonLongAt(cxJson json,cxInt idx,cxLong dv);

cxJson cxJsonArrayAt(cxJson json,cxInt idx);

cxJson cxJsonObjectAt(cxJson json,cxInt idx);

cxInt cxJsonArrayLength(cxJson json);

cxInt cxJsonObjectLength(cxJson json);
//

cxVec2f cxJsonToVec2f(cxJson json,cxVec2f dv);

cxVec2i cxJsonToVec2i(cxJson json,cxVec2i dv);

cxVec3f cxJsonToVec3f(cxJson json,cxVec3f dv);

cxSize2f cxJsonToSize2f(cxJson json,cxSize2f dv);

cxSize2i cxJsonToSize2i(cxJson json,cxSize2i dv);

cxColor4f cxJsonToColor4f(cxJson json,cxColor4f dv);

cxBox4f cxJsonToBox4f(cxJson json,cxBox4f dv);

cxBoxTex2f cxJsonToBoxTex2f(cxJson json,cxBoxTex2f dv);

cxTex2f cxJsonToTex2f(cxJson json,cxTex2f dv);

cxVec2fRange cxJsonToVec2fRangle(cxJson json,cxVec2fRange dv);

cxFloatRange cxJsonToFloatRangle(cxJson json,cxFloatRange dv);

cxColor4fRange cxJsonToColor4fRangle(cxJson json,cxColor4fRange dv);

//

cxVec2fRange cxJsonVec2fRangle(cxJson json,cxConstChars key,cxVec2fRange dv);

cxFloatRange cxJsonFloatRangle(cxJson json,cxConstChars key,cxFloatRange dv);

cxColor4fRange cxJsonColor4fRangle(cxJson json,cxConstChars key,cxColor4fRange dv);

cxVec2f cxJsonVec2f(cxJson json,cxConstChars key,cxVec2f dv);

cxVec2i cxJsonVec2i(cxJson json,cxConstChars key,cxVec2i dv);

cxVec3f cxJsonVec3f(cxJson json,cxConstChars key,cxVec3f dv);

cxSize2f cxJsonSize2f(cxJson json,cxConstChars key,cxSize2f dv);

cxSize2i cxJsonSize2i(cxJson json,cxConstChars key,cxSize2i dv);

cxColor4f cxJsonColor4f(cxJson json,cxConstChars key,cxColor4f dv);

cxBox4f cxJsonBox4f(cxJson json,cxConstChars key,cxBox4f dv);

cxBoxTex2f cxJsonBoxTex2f(cxJson json,cxConstChars key,cxBoxTex2f dv);

cxTex2f cxJsonTex2f(cxJson json,cxConstChars key,cxTex2f dv);


CX_C_END

#endif
