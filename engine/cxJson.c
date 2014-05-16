//
//  cxJson.c
//  cxEngine
//
//  Created by xuhua on 11/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxJson.h"

//if field is array
static cxBool keyIsArray(cxString key,cxChar *skey,cxInt *index)
{
    CX_ASSERT(skey != NULL && key != NULL, "args error");
    cxInt s = -1;
    cxInt e = -1;
    cxConstChars ckey = cxStringBody(key);
    cxInt len = cxStringLength(key);
    for(cxInt i=0; i < len;i++){
        if(ckey[i] == '['){
            s = i;
        }else if(ckey[i] == ']'){
            e = i;
        }
    }
    if(s == -1 || e == -1){
        memcpy(skey, ckey, len);
        skey[len] = '\0';
        return false;
    }
    cxChar num[32]={0};
    memcpy(num, ckey + s + 1, e - s - 1);
    memcpy(skey, ckey, s);
    skey[s] = '\0';
    *index = atoi(num);
    return true;
}
//support key.key key[0].filed
static json_t *cxJsonGetJson(cxJson json,cxConstChars key)
{
    cxString str = cxStringConstChars(key);
    cxArray list = cxStringSplit(str, ".");
    json_t *pv = CX_JSON_PTR(json);
    cxInt index = 0;
    cxChar skey[CX_HASH_MAX_KEY_LENGTH]={0};
    json_t *rv = NULL;
    CX_ARRAY_FOREACH(list, e){
        if(keyIsArray(cxArrayObject(e), skey, &index)){
            pv = strlen(skey) > 0 ? json_object_get(pv, skey) : pv;
            CX_ASSERT(json_is_array(pv), "json must is array");
            rv = json_array_get(pv, index);
        }else{
            rv = json_object_get(pv, skey);
        }
        if(rv == NULL){
            break;
        }
        pv = rv;
    }
    return rv;
}

CX_OBJECT_TYPE(cxJson, cxObject)
{
    
}
CX_OBJECT_INIT(cxJson, cxObject)
{
    
}
CX_OBJECT_FREE(cxJson, cxObject)
{
    if(this->json != NULL){
        json_decref(this->json);
    }
}
CX_OBJECT_TERM(cxJson, cxObject)
//

//{"vx":0,"vy":0,"rx":0,"ry":0}
cxVec2fRange cxJsonToVec2fRangle(cxJson json,cxVec2fRange dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.v.x = cxJsonDouble(json, "vx", dv.v.x);
    dv.v.y = cxJsonDouble(json, "vy", dv.v.y);
    dv.r.x = cxJsonDouble(json, "rx", dv.r.x);
    dv.r.y = cxJsonDouble(json, "ry", dv.r.y);
    return dv;
}

//{"v":0,"r":0}
cxFloatRange cxJsonToFloatRangle(cxJson json,cxFloatRange dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.v = cxJsonDouble(json, "v", dv.v);
    dv.r = cxJsonDouble(json, "r", dv.r);
    return dv;
}

//{"w":0,"h":0}
cxSize2f cxJsonToSize2f(cxJson json,cxSize2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.w = cxJsonDouble(json, "w", dv.w);
    dv.h = cxJsonDouble(json, "h", dv.h);
    return dv;
}

//{"w":0,"h":0}
cxSize2i cxJsonToSize2i(cxJson json,cxSize2i dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.w = cxJsonInt(json, "w", dv.w);
    dv.h = cxJsonInt(json, "h", dv.h);
    return dv;
}

//{"x":0,"y":0,"z":0}
cxVec3f cxJsonToVec3f(cxJson json,cxVec3f dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.x = cxJsonDouble(json, "x", dv.x);
    dv.y = cxJsonDouble(json, "y", dv.y);
    dv.z = cxJsonDouble(json, "z", dv.z);
    return dv;
}

//{"x":0,"y":0}
cxVec2i cxJsonToVec2i(cxJson json,cxVec2i dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.x = cxJsonInt(json, "x", dv.x);
    dv.y = cxJsonInt(json, "y", dv.y);
    return dv;
}

//{"x":0,"y":0}
cxVec2f cxJsonToVec2f(cxJson json,cxVec2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.x = cxJsonDouble(json, "x", dv.x);
    dv.y = cxJsonDouble(json, "y", dv.y);
    return dv;
}

//{"vr":0,"vg":0,"vb":0,"va":0,"rr":0,"rg":0,"rb":0,"ra":0}
cxColor4fRange cxJsonToColor4fRangle(cxJson json,cxColor4fRange dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.v.r = cxJsonDouble(json, "vr", dv.v.r);
    dv.v.g = cxJsonDouble(json, "vg", dv.v.g);
    dv.v.b = cxJsonDouble(json, "vb", dv.v.b);
    dv.v.a = cxJsonDouble(json, "va", dv.v.a);
    dv.r.r = cxJsonDouble(json, "rr", dv.r.r);
    dv.r.g = cxJsonDouble(json, "rg", dv.r.g);
    dv.r.b = cxJsonDouble(json, "rb", dv.r.b);
    dv.r.a = cxJsonDouble(json, "ra", dv.r.a);
    return dv;
}

//{"r":0,"g":0,"b":0,"a":0}
cxColor4f cxJsonToColor4f(cxJson json,cxColor4f dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.r = cxJsonDouble(json, "r", dv.r);
    dv.g = cxJsonDouble(json, "g", dv.g);
    dv.b = cxJsonDouble(json, "b", dv.b);
    dv.a = cxJsonDouble(json, "a", dv.a);
    return dv;
}

//{"l":0,"r":0,"t":0,"b":0}
cxBox4f cxJsonToBox4f(cxJson json,cxBox4f dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.l = cxJsonDouble(json, "l", dv.l);
    dv.r = cxJsonDouble(json, "r", dv.r);
    dv.t = cxJsonDouble(json, "t", dv.t);
    dv.b = cxJsonDouble(json, "b", dv.b);
    return dv;
}

//{"u":0,"v":0}
cxTex2f cxJsonToTex2f(cxJson json,cxTex2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.u = cxJsonDouble(json, "u", dv.u);
    dv.v = cxJsonDouble(json, "v", dv.v);
    return dv;
}

//{"lb":{"u":0,"v":0},"rb":{"u":0,"v":0},"lt":{"u":0,"v":0},"rt":{"u":0,"v":0}}
cxBoxTex2f cxJsonToBoxTex2f(cxJson json,cxBoxTex2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    dv.lb = cxJsonTex2f(json, "lb", dv.lb);
    dv.rb = cxJsonTex2f(json, "rb", dv.rb);
    dv.lt = cxJsonTex2f(json, "lt", dv.lt);
    dv.rt = cxJsonTex2f(json, "rt", dv.rt);
    return dv;
}

/////////////////////////////////////////////

cxVec2fRange cxJsonVec2fRangle(cxJson json,cxConstChars key,cxVec2fRange dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToVec2fRangle(obj, dv);
}

cxFloatRange cxJsonFloatRangle(cxJson json,cxConstChars key,cxFloatRange dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToFloatRangle(obj, dv);
}

cxSize2f cxJsonSize2f(cxJson json,cxConstChars key,cxSize2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToSize2f(obj, dv);
}

cxSize2i cxJsonSize2i(cxJson json,cxConstChars key,cxSize2i dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToSize2i(obj, dv);
}

cxVec3f cxJsonVec3f(cxJson json,cxConstChars key,cxVec3f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToVec3f(obj, dv);
}

cxVec2i cxJsonVec2i(cxJson json,cxConstChars key,cxVec2i dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToVec2i(obj, dv);
}

cxVec2f cxJsonVec2f(cxJson json,cxConstChars key,cxVec2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToVec2f(obj, dv);
}

//{"vr":0,"vg":0,"vb":0,"va":0,"rr":0,"rg":0,"rb":0,"ra":0}
cxColor4fRange cxJsonColor4fRangle(cxJson json,cxConstChars key,cxColor4fRange dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToColor4fRangle(obj, dv);
}

//{"r":0,"g":0,"b":0,"a":0}
cxColor4f cxJsonColor4f(cxJson json,cxConstChars key,cxColor4f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToColor4f(obj, dv);
}

//{"l":0,"r":0,"t":0,"b":0}
cxBox4f cxJsonBox4f(cxJson json,cxConstChars key,cxBox4f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToBox4f(obj, dv);
}

//{"u":0,"v":0}
cxTex2f cxJsonTex2f(cxJson json,cxConstChars key,cxTex2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToTex2f(obj, dv);
}

//{"lb":{"u":0,"v":0},"rb":{"u":0,"v":0},"lt":{"u":0,"v":0},"rt":{"u":0,"v":0}}
cxBoxTex2f cxJsonBoxTex2f(cxJson json,cxConstChars key,cxBoxTex2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToBoxTex2f(obj, dv);
}

static cxDouble cxJsonDoubleWithString(json_t *json)
{
    CX_ASSERT(json_is_string(json), "json must is string type");
    cxEngine engine = cxEngineInstance();
    cxConstChars str = json_string_value(json);
    cxInt len = strlen(str);
    cxChar num[16]={0};
    if(str[len - 1] == 'w'){
        memcpy(num, str, len - 1);
        return atof(num) * engine->winsize.w;
    }else if(str[len - 1] == 'h'){
        memcpy(num, str, len - 1);
        return atof(num) * engine->winsize.h;
    }
    return atof(str);
}

cxBool cxJsonExists(cxJson json,cxConstChars key)
{
    CX_RETURN(json == NULL, false);
    return json_object_get(CX_JSON_PTR(json), key) != NULL;
}

cxBool cxJsonIsInt(cxJson json)
{
    CX_RETURN(json == NULL, false);
    return json_is_integer(CX_JSON_PTR(json));
}

cxBool cxJsonIsDouble(cxJson json)
{
    CX_RETURN(json == NULL, false);
    return json_is_real(CX_JSON_PTR(json));
}

cxBool cxJsonIsNumber(cxJson json)
{
    CX_RETURN(json == NULL, false);
    return json_is_number(CX_JSON_PTR(json));
}

cxBool cxJsonIsString(cxJson json)
{
    CX_RETURN(json == NULL, false);
    return json_is_string(CX_JSON_PTR(json));
}

cxBool cxJsonIsBool(cxJson json)
{
    CX_RETURN(json == NULL, false);
    return json_is_boolean(CX_JSON_PTR(json));
}

cxBool cxJsonIsArray(cxJson json)
{
    CX_RETURN(json == NULL, false);
    return json_is_array(CX_JSON_PTR(json));
}

cxBool cxJsonIsObject(cxJson json)
{
    CX_RETURN(json == NULL, false);
    return json_is_object(CX_JSON_PTR(json));
}

cxJson cxJsonCreate(cxString json)
{
    CX_ASSERT(json != NULL, "args error");
    cxJson this = CX_CREATE(cxJson);
    json_error_t error = {0};
    if(cxStringLength(json) <= 0){
        return NULL;
    }
    this->json = json_loadb(cxStringBody(json), cxStringLength(json), JSON_DECODE_ANY, &error);
    CX_ASSERT(this->json != NULL, "cxJson load error (%d:%d) %s:%s",error.line,error.column,error.source,error.text);
    return this;
}
//alloc must release
cxJson cxJsonAttachAlloc(json_t *json)
{
    CX_ASSERT(json != NULL, "json ptr error");
    cxJson this = CX_ALLOC(cxJson);
    this->json = json;
    json_incref(this->json);
    return this;
}

cxJson cxJsonToArray(cxJson json)
{
    CX_ASSERT(json != NULL, "json error");
    if(!cxJsonIsArray(json)){
        return NULL;
    }
    return json;
}

cxJson cxJsonToObject(cxJson json)
{
    CX_ASSERT(json != NULL, "json error");
    if(!cxJsonIsObject(json)){
        return NULL;
    }
    return json;
}

cxInt cxJsonToInt(cxJson json,cxInt dv)
{
    CX_ASSERT(json != NULL, "json error");
    if(json_is_string(CX_JSON_PTR(json))){
        return atoi(json_string_value(CX_JSON_PTR(json)));
    }
    if(!json_is_integer(CX_JSON_PTR(json))){
        return dv;
    }
    return (cxInt)json_integer_value(CX_JSON_PTR(json));
}

cxConstChars cxJsonToConstChars(cxJson json)
{
    CX_ASSERT(json != NULL, "json error");
    if(!json_is_string(CX_JSON_PTR(json))){
        return NULL;
    }
    return json_string_value(CX_JSON_PTR(json));
}

cxString cxJsonToString(cxJson json)
{
    CX_ASSERT(json != NULL, "json error");
    cxConstChars str = cxJsonToConstChars(json);
    return (str != NULL)?cxStringConstChars(str) : NULL;
}

cxDouble cxJsonToDouble(cxJson json,cxDouble dv)
{
    CX_ASSERT(json != NULL, "json error");
    if(json_is_string(CX_JSON_PTR(json))){
        return cxJsonDoubleWithString(CX_JSON_PTR(json));
    }
    if(!json_is_number(CX_JSON_PTR(json))){
        return dv;
    }
    return json_number_value(CX_JSON_PTR(json));
}

cxLong cxJsonToLong(cxJson json,cxLong dv)
{
    CX_ASSERT(json != NULL, "json error");
    if(json_is_string(CX_JSON_PTR(json))){
        return atol(json_string_value(CX_JSON_PTR(json)));
    }
    if(!json_is_number(CX_JSON_PTR(json))){
        return dv;
    }
    return (cxLong)json_integer_value(CX_JSON_PTR(json));
}

cxBool cxJsonToBool(cxJson json,cxBool dv)
{
    CX_ASSERT(json != NULL, "json error");
    if(json_is_string(CX_JSON_PTR(json))){
        return strcasecmp(json_string_value(CX_JSON_PTR(json)), "true") == 0;
    }
    if(!json_is_boolean(CX_JSON_PTR(json))){
        return dv;
    }
    return json_is_true(CX_JSON_PTR(json));
}

cxInt cxJsonIntAt(cxJson json,cxInt idx,cxInt dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return atoi(json_string_value(v));
    }
    return (cxInt)json_integer_value(v);
}

cxConstChars cxJsonConstCharsAt(cxJson json,cxInt idx)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return 0;
    }
    return json_string_value(v);
}

cxString cxJsonStringAt(cxJson json,cxInt idx)
{
    CX_ASSERT(json != NULL, "json error");
    cxConstChars str = cxJsonConstCharsAt(json, idx);
    return (str != NULL)?cxStringConstChars(str) : NULL;
}

cxBool cxJsonBoolAt(cxJson json,cxInt idx,cxBool dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return strcasecmp(json_string_value(v),"true") == 0;
    }
    return json_is_true(v);
}

cxDouble cxJsonDoubleAt(cxJson json,cxInt idx,cxDouble dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return cxJsonDoubleWithString(v);
    }
    return json_number_value(v);
}

cxLong cxJsonLongAt(cxJson json,cxInt idx,cxLong dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return atol(json_string_value(v));
    }
    return (cxLong)json_integer_value(v);
}

cxJson cxJsonArrayAt(cxJson json,cxInt idx)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return NULL;
    }
    if(!json_is_array(v)){
        return NULL;
    }
    cxJson rv = CX_CREATE(cxJson);
    rv->json = json_incref(v);
    return rv;
}

cxInt cxJsonObjectLength(cxJson json)
{
    CX_RETURN(json == NULL, 0);
    return json_object_size(CX_JSON_PTR(json));
}

cxInt cxJsonArrayLength(cxJson json)
{
    CX_RETURN(json == NULL, 0);
    return json_array_size(CX_JSON_PTR(json));
}

cxJson cxJsonObjectAt(cxJson json,cxInt idx)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return NULL;
    }
    if(!json_is_object(v)){
        return NULL;
    }
    cxJson rv = CX_CREATE(cxJson);
    rv->json = json_incref(v);
    return rv;
}

cxJson cxJsonArray(cxJson json,cxConstChars key)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    if(v == NULL){
        return NULL;
    }
    if(!json_is_array(v)){
        return NULL;
    }
    cxJson rv = CX_CREATE(cxJson);
    rv->json = json_incref(v);
    return rv;
}

cxJson cxJsonObject(cxJson json,cxConstChars key)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    if(v == NULL){
        return NULL;
    }
    if(!json_is_object(v)){
        return NULL;
    }
    cxJson rv = CX_CREATE(cxJson);
    rv->json = json_incref(v);
    return rv;
}

cxBool cxJsonBool(cxJson json,cxConstChars key,cxBool dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return strcasecmp(json_string_value(v), "true") == 0;
    }
    return json_is_true(v);
}

cxConstChars cxJsonConstChars(cxJson json,cxConstChars key)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    if(v == NULL){
        return NULL;
    }
    return json_string_value(v);
}

cxString cxJsonString(cxJson json,cxConstChars key)
{
    CX_ASSERT(json != NULL, "json error");
    cxConstChars str = cxJsonConstChars(json, key);
    return (str != NULL && strlen(str) > 0)?cxStringConstChars(str) : NULL;
}

cxDouble cxJsonDouble(cxJson json,cxConstChars key,cxDouble dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return cxJsonDoubleWithString(v);
    }
    return json_number_value(v);
}

cxLong cxJsonLong(cxJson json,cxConstChars key,cxLong dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return atol(json_string_value(v));
    }
    return (cxLong)json_integer_value(v);
}

cxInt cxJsonInt(cxJson json,cxConstChars key,cxInt dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return atoi(json_string_value(v));
    }
    return (cxInt)json_integer_value(v);
}










