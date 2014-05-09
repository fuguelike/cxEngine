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
    cxInt s,e = -1;
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

cxBool cxJsonExists(cxJson json,cxConstChars key)
{
    if(!cxJsonIsObject(json)){
        return false;
    }
    return json_object_get(CX_JSON_PTR(json), key) != NULL;
}

cxBool cxJsonIsInt(cxJson json)
{
    return json_is_integer(CX_JSON_PTR(json));
}

cxBool cxJsonIsDouble(cxJson json)
{
    return json_is_real(CX_JSON_PTR(json));
}

cxBool cxJsonIsNumber(cxJson json)
{
    return json_is_number(CX_JSON_PTR(json));
}

cxBool cxJsonIsString(cxJson json)
{
    return json_is_string(CX_JSON_PTR(json));
}

cxBool cxJsonIsBool(cxJson json)
{
    return json_is_boolean(CX_JSON_PTR(json));
}

cxBool cxJsonIsArray(cxJson json)
{
    return json_is_array(CX_JSON_PTR(json));
}

cxBool cxJsonIsObject(cxJson json)
{
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
    if(!json_is_string(CX_JSON_PTR(json))){
        return NULL;
    }
    return json_string_value(CX_JSON_PTR(json));
}

cxString cxJsonToString(cxJson json)
{
    cxConstChars str = cxJsonToConstChars(json);
    return (str != NULL)?cxStringConstChars(str) : NULL;
}

cxDouble cxJsonToDouble(cxJson json,cxDouble dv)
{
    if(json_is_string(CX_JSON_PTR(json))){
        return atof(json_string_value(CX_JSON_PTR(json)));
    }
    if(!json_is_number(CX_JSON_PTR(json))){
        return dv;
    }
    return json_number_value(CX_JSON_PTR(json));
}

cxLong cxJsonToLong(cxJson json,cxLong dv)
{
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
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return 0;
    }
    return json_string_value(v);
}

cxString cxJsonStringAt(cxJson json,cxInt idx)
{
    cxConstChars str = cxJsonConstCharsAt(json, idx);
    return (str != NULL)?cxStringConstChars(str) : NULL;
}

cxBool cxJsonBoolAt(cxJson json,cxInt idx,cxBool dv)
{
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
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return atof(json_string_value(v));
    }
    return json_number_value(v);
}

cxLong cxJsonLongAt(cxJson json,cxInt idx,cxLong dv)
{
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
    return json_array_size(CX_JSON_PTR(json));
}

cxJson cxJsonObjectAt(cxJson json,cxInt idx)
{
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
    json_t *v = cxJsonGetJson(json, key);
    if(v == NULL){
        return NULL;
    }
    return json_string_value(v);
}

cxString cxJsonString(cxJson json,cxConstChars key)
{
    cxConstChars str = cxJsonConstChars(json, key);
    return (str != NULL && strlen(str) > 0)?cxStringConstChars(str) : NULL;
}

cxDouble cxJsonDouble(cxJson json,cxConstChars key,cxDouble dv)
{
    json_t *v = cxJsonGetJson(json, key);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return atof(json_string_value(v));
    }
    return json_number_value(v);
}

cxLong cxJsonLong(cxJson json,cxConstChars key,cxLong dv)
{
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
    json_t *v = cxJsonGetJson(json, key);
    if(v == NULL){
        return dv;
    }
    if(json_is_string(v)){
        return atoi(json_string_value(v));
    }
    return (cxInt)json_integer_value(v);
}











