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
#include "cxCore.h"
#include "cxStr.h"
#include "cxMath.h"

CX_C_BEGIN

CX_DEF(cxJson, cxObject)
    json_t *json;
CX_END(cxJson, cxObject)

#define CX_JSON_PTR(_j_)  ((_j_)->json)

#define CX_JSON_ARRAY_EACH_BEG(_j_,_v_)                             \
if((_j_) != NULL){                                                  \
    json_t *_v_##_value_ = NULL;                                    \
    json_t *_v_##_json_= CX_JSON_PTR(_j_);                          \
    cxInt _v_##Index = 0;                                           \
    json_array_foreach(_v_##_json_, _v_##Index, _v_##_value_){      \
        cxJson _v_ = cxJsonAttachAlloc(_v_##_value_);
#define CX_JSON_ARRAY_EACH_END(_j_,_v_)                             \
        CX_RELEASE(_v_);                                            \
    }                                                               \
}

#define CX_JSON_OBJECT_EACH_BEG(_j_,_v_)                            \
if((_j_) != NULL){                                                  \
    json_t *_v_##_value_ = NULL;                                    \
    json_t *_v_##_json_= CX_JSON_PTR(_j_);                          \
    cxConstChars _v_##Key = NULL;                                   \
    json_object_foreach(_v_##_json_, _v_##Key, _v_##_value_){       \
        cxJson _v_ = cxJsonAttachAlloc(_v_##_value_);
#define CX_JSON_OBJECT_EACH_END(_j_,_v_)                            \
        CX_RELEASE(_v_);                                            \
    }                                                               \
}

CX_DEF(cxConvert, cxObject)
    cxAny func;
CX_END(cxConvert, cxObject)

cxStr cxStrFormatConvert(cxJson args);

void cxSetConvert(cxConstChars name,cxAny func);

cxConvert cxGetConvert(cxConstChars name);

cxJson cxJsonSelect(cxJson json,cxConstChars key,...);

cxBool cxKeyIsNumber(cxConstChars ckey,cxInt *num);

//return value will autorelease
json_t *jsonCreateStr(cxConstChars str);

cxJson cxJsonRead(cxConstChars src);

cxStr cxJsonDump(cxJson json);

void cxJsonPrint(cxAny pjson);

void cxJsonClear();

void cxJsonInit();

void cxJsonFree();

cxJson cxJsonCreate(cxStr json);

cxBool cxJsonIsInt(cxJson json);

cxBool cxJsonIsDouble(cxJson json);

cxBool cxJsonIsNumber(cxJson json);

cxBool cxJsonIsStr(cxJson json);

cxBool cxJsonIsBool(cxJson json);

cxBool cxJsonIsArray(cxJson json);

cxBool cxJsonIsObject(cxJson json);

cxJson cxJsonAttachAlloc(json_t *json);

cxJson cxJsonAttach(json_t *json);

cxJson cxJsonReference(json_t *json);;

cxJson cxJsonToArray(cxJson json);

cxJson cxJsonToObject(cxJson json);

cxInt cxJsonToInt(cxJson json,cxInt dv);

cxConstChars cxJsonToConstChars(cxJson json);

cxStr cxJsonToStr(cxJson json);

cxDouble cxJsonToDouble(cxJson json,cxDouble dv);

cxInt64 cxJsonToInt64(cxJson json,cxInt64 dv);

cxBool cxJsonToBool(cxJson json,cxBool dv);

cxInt cxJsonInt(cxJson json,cxConstChars key,cxInt dv);

cxConstChars cxJsonConstChars(cxJson json,cxConstChars key);

cxStr cxJsonStr(cxJson json,cxConstChars key);

cxDouble cxJsonDouble(cxJson json,cxConstChars key,cxDouble ev);

cxFloat cxJsonFloat(cxJson json,cxConstChars key,cxFloat dv);

cxInt64 cxJsonInt64(cxJson json,cxConstChars key,cxInt64 dv);

cxBool cxJsonBool(cxJson json,cxConstChars key,cxBool dv);

cxJson cxJsonArray(cxJson json,cxConstChars key);

cxJson cxJsonObject(cxJson json,cxConstChars key);

cxJson cxJsonAny(cxJson json,cxConstChars key);

cxBool cxJsonExists(cxJson json,cxConstChars key);

cxInt cxJsonIntAt(cxJson json,cxInt idx,cxInt dv);

cxConstChars cxJsonConstCharsAt(cxJson json,cxInt idx);

cxStr cxJsonStrAt(cxJson json,cxInt idx);

cxDouble cxJsonDoubleAt(cxJson json,cxInt idx,cxDouble dv);

cxBool cxJsonBoolAt(cxJson json,cxInt idx,cxBool dv);

cxInt64 cxJsonInt64At(cxJson json,cxInt idx,cxInt64 dv);

cxJson cxJsonArrayAt(cxJson json,cxInt idx);

cxJson cxJsonObjectAt(cxJson json,cxInt idx);

cxJson cxJsonAnyAt(cxJson json,cxInt idx);

cxInt cxJsonArrayLength(cxJson json);

cxInt cxJsonObjectLength(cxJson json);
//

cxJson cxJsonVec2fToJson(cxVec2f dv);
cxVec2f cxJsonToVec2f(cxJson json,cxVec2f dv);

cxJson cxJsonRange2fToJson(cxRange2f dv);
cxRange2f cxJsonToRange2f(cxJson json,cxRange2f dv);

cxJson cxJsonRange2iToJson(cxRange2i dv);
cxRange2i cxJsonToRange2i(cxJson json,cxRange2i dv);

cxJson cxJsonVec2iToJson(cxVec2i dv);
cxVec2i cxJsonToVec2i(cxJson json,cxVec2i dv);

cxJson cxJsonVec3fToJson(cxVec3f dv);
cxVec3f cxJsonToVec3f(cxJson json,cxVec3f dv);

cxJson cxJsonSize2fToJson(cxSize2f dv);
cxSize2f cxJsonToSize2f(cxJson json,cxSize2f dv);

cxJson cxJsonRect4fToJson(cxRect4f dv);
cxRect4f cxJsonToRect4f(cxJson json,cxRect4f dv);

cxSize2i cxJsonToSize2i(cxJson json,cxSize2i dv);
cxJson cxJsonSizeifToJson(cxSize2i dv);

cxJson cxJsonColor4fToJson(cxColor4f dv);
cxColor4f cxJsonToColor4f(cxJson json,cxColor4f dv);

cxColor3f cxJsonToColor3f(cxJson json,cxColor3f dv);
cxJson cxJsonColor3fTojson(cxColor3f dv);

cxBox4f cxJsonToBox4f(cxJson json,cxBox4f dv);
cxJson cxJsonBox4fTojson(cxBox4f dv);

cxBoxTex2f cxJsonToBoxTex2f(cxJson json,cxBoxTex2f dv);
cxJson cxJsonBoxTex2fToJson(cxBoxTex2f dv);

cxTex2f cxJsonToTex2f(cxJson json,cxTex2f dv);
cxJson cxJsonTex2fToJson(cxTex2f dv);

cxVec2fRange cxJsonVec2fRange(cxJson json,cxConstChars key,cxVec2fRange dv);
cxVec2fRange cxJsonToVec2fRange(cxJson json,cxVec2fRange dv);

cxJson cxJsonFloatRangeToJson(cxFloatRange dv);
cxFloatRange cxJsonToFloatRange(cxJson json,cxFloatRange dv);

cxJson cxJsonColor4fRangeToJson(cxColor4fRange dv);
cxColor4fRange cxJsonToColor4fRange(cxJson json,cxColor4fRange dv);

cxJson cxJsonvec3fRangeToJson(cxVec3fRange dv);
cxVec3fRange cxJsonToVec3fRange(cxJson json,cxVec3fRange dv);

cxJson cxJsonIntToJson(cxInt dv);

cxJson cxJsonFloatToJson(cxFloat dv);

cxJson cxJsonDoubleToJson(cxDouble dv);

cxJson cxJsonInt64ToJson(cxInt64 dv);

cxJson cxJsonConstCharsToJson(cxConstChars dv);

cxJson cxJsonStrToJson(cxStr dv);

//
cxJson cxJsonVec2fRangeToJson(cxVec2fRange v);

cxFloatRange cxJsonFloatRange(cxJson json,cxConstChars key,cxFloatRange dv);

cxColor4fRange cxJsonColor4fRange(cxJson json,cxConstChars key,cxColor4fRange dv);

cxVec2f cxJsonVec2f(cxJson json,cxConstChars key,cxVec2f dv);

cxRange2f cxJsonRange2f(cxJson json,cxConstChars key,cxRange2f dv);

cxVec2i cxJsonVec2i(cxJson json,cxConstChars key,cxVec2i dv);

cxVec3f cxJsonVec3f(cxJson json,cxConstChars key,cxVec3f dv);

cxSize2f cxJsonSize2f(cxJson json,cxConstChars key,cxSize2f dv);

cxRect4f cxJsonRect4f(cxJson json,cxConstChars key,cxRect4f dv);

cxSize2i cxJsonSize2i(cxJson json,cxConstChars key,cxSize2i dv);

cxColor4f cxJsonColor4f(cxJson json,cxConstChars key,cxColor4f dv);

cxBox4f cxJsonBox4f(cxJson json,cxConstChars key,cxBox4f dv);

cxBoxTex2f cxJsonBoxTex2f(cxJson json,cxConstChars key,cxBoxTex2f dv);

cxTex2f cxJsonTex2f(cxJson json,cxConstChars key,cxTex2f dv);
//

cxJson cxJsonCreateObject();

cxJson cxJsonCreateVec2f(cxVec2f pos);

cxJson cxJsonCreateVec2i(cxVec2i pos);

cxJson cxJsonCreateSize2f(cxSize2f size);

void cxJsonSetFormat(cxJson json,cxConstChars key,cxConstChars format,...);

void cxJsonSetConstChars(cxJson json,cxConstChars key,cxConstChars v);

void cxJsonSetStr(cxJson json,cxConstChars key,cxStr v);

void cxJsonSetJson(cxJson json,cxConstChars key,cxJson v);

void cxJsonSetVec2i(cxJson json,cxConstChars key,cxVec2i idx);

void cxJsonSetInt(cxJson json,cxConstChars key,cxInt v);

void cxJsonSetInt64(cxJson json,cxConstChars key,cxInt64 v);

void cxJsonSetFloat(cxJson json,cxConstChars key,cxFloat v);

void cxJsonSetDouble(cxJson json,cxConstChars key,cxDouble v);

void cxJsonSetBool(cxJson json,cxConstChars key,cxBool v);

cxJson cxJsonCreateArray();

void cxJsonAppendFormat(cxJson json,cxConstChars format,...);

void cxJsonAppendConstChars(cxJson json,cxConstChars v);

void cxJsonAppendStr(cxJson json,cxStr v);

void cxJsonAppendInt(cxJson json,cxInt v);

void cxJsonAppendInt64(cxJson json,cxInt64 v);

void cxJsonAppendDouble(cxJson json,cxDouble v);

void cxJsonAppendBool(cxJson json,cxBool v);

void cxJsonAppendJson(cxJson json,cxJson v);

//

cxAny cxJsonDecode(cxJson this);

void cxJsonSetAESKey(cxStr v);

cxStr cxJsonAESEncode(cxJson json);

cxJson cxJsonAESDecode(cxStr data);

cxStr cxJsonAESEncodeWithKey(cxJson json,cxStr key);

cxJson cxJsonAESDecodeWithKey(cxStr data,cxStr key);

cxAny cxJsonTocxObject(cxJson v);

cxAny cxJsonMakeObject(cxJson json);

void cxJsonRunSetter(cxAny pobj,cxJson json);

CX_C_END

#endif
