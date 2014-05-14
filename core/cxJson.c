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
        pv = rv;
    }
    return rv;
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

//{"vx":0,"vy":0,"rx":0,"ry":0}
cxVec2fRange cxJsonVec2fRangle(cxJson json,cxConstChars key,cxVec2fRange dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.v.x = cxJsonDouble(obj, "vx", dv.v.x);
    dv.v.y = cxJsonDouble(obj, "vy", dv.v.y);
    dv.r.x = cxJsonDouble(obj, "rx", dv.r.x);
    dv.r.y = cxJsonDouble(obj, "ry", dv.r.y);
    return dv;
}

//{"v":0,"r":0}
cxFloatRange cxJsonFloatRangle(cxJson json,cxConstChars key,cxFloatRange dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.v = cxJsonDouble(obj, "v", dv.v);
    dv.r = cxJsonDouble(obj, "r", dv.r);
    return dv;
}

//{"w":0,"h":0}
cxSize2f cxJsonSize2f(cxJson json,cxConstChars key,cxSize2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.w = cxJsonDouble(obj, "w", dv.w);
    dv.h = cxJsonDouble(obj, "h", dv.h);
    return dv;
}

//{"x":0,"y":0,"z":0}
cxVec3f cxJsonVec3f(cxJson json,cxConstChars key,cxVec3f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.x = cxJsonDouble(obj, "x", dv.x);
    dv.y = cxJsonDouble(obj, "y", dv.y);
    dv.z = cxJsonDouble(obj, "z", dv.z);
    return dv;
}

//{"x":0,"y":0}
cxVec2i cxJsonVec2i(cxJson json,cxConstChars key,cxVec2i dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.x = cxJsonInt(obj, "x", dv.x);
    dv.y = cxJsonInt(obj, "y", dv.y);
    return dv;
}

//{"x":0,"y":0}
cxVec2f cxJsonVec2f(cxJson json,cxConstChars key,cxVec2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.x = cxJsonDouble(obj, "x", dv.x);
    dv.y = cxJsonDouble(obj, "y", dv.y);
    return dv;
}

//{"vr":0,"vg":0,"vb":0,"va":0,"rr":0,"rg":0,"rb":0,"ra":0}
cxColor4fRange cxJsonColor4fRangle(cxJson json,cxConstChars key,cxColor4fRange dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.v.r = cxJsonDouble(obj, "vr", dv.v.r);
    dv.v.g = cxJsonDouble(obj, "vg", dv.v.g);
    dv.v.b = cxJsonDouble(obj, "vb", dv.v.b);
    dv.v.a = cxJsonDouble(obj, "va", dv.v.a);
    dv.r.r = cxJsonDouble(obj, "rr", dv.r.r);
    dv.r.g = cxJsonDouble(obj, "rg", dv.r.g);
    dv.r.b = cxJsonDouble(obj, "rb", dv.r.b);
    dv.r.a = cxJsonDouble(obj, "ra", dv.r.a);
    return dv;
}

//{"r":0,"g":0,"b":0,"a":0}
cxColor4f cxJsonColor4f(cxJson json,cxConstChars key,cxColor4f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.r = cxJsonDouble(obj, "r", dv.r);
    dv.g = cxJsonDouble(obj, "g", dv.g);
    dv.b = cxJsonDouble(obj, "b", dv.b);
    dv.a = cxJsonDouble(obj, "a", dv.a);
    return dv;
}

//{"l":0,"r":0,"t":0,"b":0}
cxBox4f cxJsonBox4f(cxJson json,cxConstChars key,cxBox4f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.l = cxJsonDouble(obj, "l", dv.l);
    dv.r = cxJsonDouble(obj, "r", dv.r);
    dv.t = cxJsonDouble(obj, "t", dv.t);
    dv.b = cxJsonDouble(obj, "b", dv.b);
    return dv;
}

//{"u":0,"v":0}
cxTex2f cxJsonTex2f(cxJson json,cxConstChars key,cxTex2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.u = cxJsonDouble(obj, "u", dv.u);
    dv.v = cxJsonDouble(obj, "v", dv.v);
    return dv;
}

//{"lb":{"u":0,"v":0},"rb":{"u":0,"v":0},"lt":{"u":0,"v":0},"rt":{"u":0,"v":0}}
cxBoxTex2f cxJsonBoxTex2f(cxJson json,cxConstChars key,cxBoxTex2f dv)
{
    CX_ASSERT(json != NULL, "json error");
    cxJson obj = cxJsonObject(json, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    dv.lb = cxJsonTex2f(obj, "lb", dv.lb);
    dv.rb = cxJsonTex2f(obj, "rb", dv.rb);
    dv.lt = cxJsonTex2f(obj, "lt", dv.lt);
    dv.rt = cxJsonTex2f(obj, "rt", dv.rt);
    return dv;
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

cxInt cxJsonArrayLength(cxJson json)
{
    CX_ASSERT(json != NULL, "json error");
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











