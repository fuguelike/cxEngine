//
//  cxJson.c
//  cxEngine
//
//  Created by xuhua on 11/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

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
    this->json = json_loadb(cxStringBody(json), cxStringLength(json), JSON_DECODE_ANY, &error);
    if(this->json == NULL){
        CX_ERROR("cxJson load error (%d:%d) %s:%s",error.line,error.column,error.source,error.text);
        return NULL;
    }
    return this;
}

cxInt cxJsonIntAt(cxJson json,cxInt idx)
{
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        CX_ERROR("get json index %d failed",idx);
        return 0;
    }
    return (cxInt)json_integer_value(v);
}

cxConstChars cxJsonStringAt(cxJson json,cxInt idx)
{
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        CX_ERROR("get json index %d failed",idx);
        return 0;
    }
    return json_string_value(v);
}

cxDouble cxJsonDoubleAt(cxJson json,cxInt idx)
{
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        CX_ERROR("get json index %d failed",idx);
        return 0;
    }
    return json_number_value(v);
}

cxLong cxJsonLongAt(cxJson json,cxInt idx)
{
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        CX_ERROR("get json index %d failed",idx);
        return 0;
    }
    return (cxLong)json_integer_value(v);
}

cxJson cxJsonArrayAt(cxJson json,cxInt idx)
{
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    if(v == NULL){
        CX_ERROR("get json index %d failed",idx);
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
        CX_ERROR("get json index %d failed",idx);
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
        CX_ERROR("get json key %s failed",key);
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
        CX_ERROR("get json key %s failed",key);
        return NULL;
    }
    if(!json_is_object(v)){
        return NULL;
    }
    cxJson rv = CX_CREATE(cxJson);
    rv->json = json_incref(v);
    return rv;
}

cxConstChars cxJsonString(cxJson json,cxConstChars key)
{
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
    if(v == NULL){
        CX_ERROR("get json key %s failed",key);
        return NULL;
    }
    return json_string_value(v);
}

cxDouble cxJsonDouble(cxJson json,cxConstChars key)
{
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
    if(v == NULL){
        CX_ERROR("get json key %s failed",key);
        return 0;
    }
    return json_number_value(v);
}

cxLong cxJsonLong(cxJson json,cxConstChars key)
{
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
    if(v == NULL){
        CX_ERROR("get json key %s failed",key);
        return 0;
    }
    return (cxLong)json_integer_value(v);
}

cxInt cxJsonInt(cxJson json,cxConstChars key)
{
    json_t *v = json_object_get(CX_JSON_PTR(json), key);
    if(v == NULL){
        CX_ERROR("get json key %s failed",key);
        return 0;
    }
    return (cxInt)json_integer_value(v);
}

