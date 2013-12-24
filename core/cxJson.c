//
//  cxJson.c
//  cxEngine
//
//  Created by xuhua on 11/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxJson.h"

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

cxJson cxJsonCreate(cxString json)
{
    CX_ASSERT(json != NULL, "args error");
    cxJson this = CX_CREATE(cxJson);
    json_error_t error = {0};
    if(cxStringLength(json) <= 0){
        return this;
    }
    this->json = json_loadb(cxStringBody(json), cxStringLength(json), JSON_DECODE_ANY, &error);
    CX_ASSERT(this->json != NULL, "cxJson load error (%d:%d) %s:%s",error.line,error.column,error.source,error.text);
    return this;
}

cxBool cxJsonPush(cxJson json)
{
    if(json_is_string(CX_JSON_PTR(json))){
        lua_pushstring(gL, cxJsonToString(json));
        return true;
    }
    if(json_is_integer(CX_JSON_PTR(json))){
        lua_pushinteger(gL, cxJsonToInt(json, 0));
        return true;
    }
    if(json_is_number(CX_JSON_PTR(json))){
        lua_pushnumber(gL, cxJsonToDouble(json, 0));
        return true;
    }
    if(json_is_object(CX_JSON_PTR(json))){
        lua_newtable(gL);
        cxConstChars key = NULL;
        json_t *value = NULL;
        json_object_foreach(CX_JSON_PTR(json), key, value){
            if(json_is_integer(value)){
                lua_pushinteger(gL,(int)json_integer_value(value));
            }else if(json_is_string(value)){
                lua_pushstring(gL, json_string_value(value));
            }else if(json_is_number(value)){
                lua_pushnumber(gL, json_number_value(value));
            }else if(json_is_true(value)){
                lua_pushboolean(gL, 1);
            }else if(json_is_false(value)){
                lua_pushboolean(gL, 0);
            }else if(json_is_null(value)){
                lua_pushnil(gL);
            }else if(json_is_real(value)){
                lua_pushnumber(gL, json_real_value(value));
            }
            lua_setfield(gL, -2, key);
        }
        return true;
    }
    return false;
}

cxInt cxJsonToInt(cxJson json,cxInt dv)
{
    if(!json_is_integer(CX_JSON_PTR(json))){
        return dv;
    }
    return (cxInt)json_integer_value(CX_JSON_PTR(json));
}

cxConstChars cxJsonToString(cxJson json)
{
    if(!json_is_string(CX_JSON_PTR(json))){
        return NULL;
    }
    return json_string_value(CX_JSON_PTR(json));
}

cxDouble cxJsonToDouble(cxJson json,cxDouble dv)
{
    if(!json_is_number(CX_JSON_PTR(json))){
        return dv;
    }
    return json_number_value(CX_JSON_PTR(json));
}

cxLong cxJsonToLong(cxJson json,cxLong dv)
{
    if(!json_is_number(CX_JSON_PTR(json))){
        return dv;
    }
    return (cxLong)json_integer_value(CX_JSON_PTR(json));
}

cxBool cxJsonToBool(cxJson json,cxBool dv)
{
    if(!json_is_boolean(CX_JSON_PTR(json))){
        return dv;
    }
    return json_is_true(CX_JSON_PTR(json));
}

cxInt cxJsonIntAt(cxJson json,cxInt idx)
{
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return 0;
    }
    return (cxInt)json_integer_value(v);
}

cxConstChars cxJsonStringAt(cxJson json,cxInt idx)
{
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return 0;
    }
    return json_string_value(v);
}

cxBool cxJsonBoolAt(cxJson json,cxInt idx,cxBool dv)
{
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return dv;
    }
    return json_is_true(v);
}

cxDouble cxJsonDoubleAt(cxJson json,cxInt idx,cxDouble dv)
{
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return dv;
    }
    return json_number_value(v);
}

cxLong cxJsonLongAt(cxJson json,cxInt idx,cxLong dv)
{
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        return dv;
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
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
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
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
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
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
    if(v == NULL){
        return dv;
    }
    return json_is_true(v);
}

cxConstChars cxJsonString(cxJson json,cxConstChars key)
{
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
    if(v == NULL){
        return NULL;
    }
    return json_string_value(v);
}

cxDouble cxJsonDouble(cxJson json,cxConstChars key,cxDouble dv)
{
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
    if(v == NULL){
        return dv;
    }
    return json_number_value(v);
}

cxLong cxJsonLong(cxJson json,cxConstChars key,cxLong dv)
{
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
    if(v == NULL){
        return dv;
    }
    return (cxLong)json_integer_value(v);
}

cxInt cxJsonInt(cxJson json,cxConstChars key,cxInt dv)
{
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
    if(v == NULL){
        return dv;
    }
    return (cxInt)json_integer_value(v);
}

