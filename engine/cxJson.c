//
//  cxJson.c
//  cxEngine
//
//  Created by xuhua on 11/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxJson.h"

static json_t *global = NULL;

void cxJsonRegisterConstChars(cxConstChars key,cxConstChars value)
{
    CX_ASSERT(cxConstCharsOK(key), "key error");
    CX_ASSERT(cxConstCharsOK(value), "value error");
    json_object_set(global, key, json_string(value));
}

void cxJsonRegisterString(cxConstChars key,cxString value)
{
    CX_ASSERT(cxConstCharsOK(key), "key error");
    CX_ASSERT(cxStringOK(value), "value error");
    json_object_set(global, key, json_string(cxStringBody(value)));
}

void cxJsonRegisterDouble(cxConstChars key,cxDouble value)
{
    CX_ASSERT(cxConstCharsOK(key), "key error");
    json_object_set(global, key, json_real(value));
}

void cxJsonRegisterInt(cxConstChars key,cxInt value)
{
    CX_ASSERT(cxConstCharsOK(key), "key error");
    json_object_set(global, key, json_integer(value));
}

void cxJsonRegisterLong(cxConstChars key,cxLong value)
{
    CX_ASSERT(cxConstCharsOK(key), "key error");
    json_object_set(global, key, json_integer(value));
}

static json_t *cxJsonGetGlobalJson(json_t *v)
{
    json_t *key = json_object_get(v, "_G");
    CX_ASSERT(v != NULL, "v error");
    if(key != NULL && json_is_string(key)){
        cxConstChars gKey = json_string_value(key);
        v = json_object_get(global, gKey);
        CX_ASSERT(v != NULL, "globale key(%s) not regster",gKey);
    }
    return v;
}

void cxJsonDump()
{
    cxChars json = json_dumps(global, JSON_ENCODE_ANY);
    CX_LOGGER("_G=%s",json);
    allocator->free(json);
    
}

void cxJsonInit()
{
    json_set_alloc_funcs(cxMalloc, cxFree);
    global = json_object();
}

void cxJsonFree()
{
    json_decref(global);
}

//if field is array
static cxBool keyIsArray(cxString key,cxChars skey,cxInt *index)
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
    cxChar skey[CX_MAX_KEY]={0};
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

static cxJson jsonToArray(json_t *v)
{
    CX_RETURN(v == NULL,NULL);
    if(!json_is_array(v)){
        return NULL;
    }
    cxJson rv = CX_CREATE(cxJson);
    rv->json = json_incref(v);
    return rv;
}

static cxJson jsonToObject(json_t *v)
{
    CX_RETURN(v == NULL,NULL);
    if(!json_is_object(v)){
        return NULL;
    }
    cxJson rv = CX_CREATE(cxJson);
    rv->json = json_incref(v);
    return rv;
}

static cxInt jsonToInt(json_t *v,cxInt dv)
{
    CX_RETURN(v == NULL,dv);
    if(json_is_object(v)){
        v = cxJsonGetGlobalJson(v);
    }
    if(json_is_string(v)){
        return atoi(json_string_value(v));
    }
    return (cxInt)json_integer_value(v);
}

static cxConstChars jsonToConstChars(json_t *v)
{
    CX_RETURN(v == NULL, NULL);
    cxConstChars str = NULL;
    if(json_is_object(v)){
        v = cxJsonGetGlobalJson(v);
    }
    if(json_is_string(v)){
        str = json_string_value(v);
    }
    return str;
}

static cxString jsonToString(json_t *v)
{
    CX_RETURN(v == NULL, NULL);
    cxConstChars str = jsonToConstChars(v);
    return (str != NULL && strlen(str) > 0)?cxStringConstChars(str) : NULL;
}

static cxDouble jsonToDouble(json_t *v,cxDouble dv)
{
    CX_RETURN(v == NULL,dv);
    if(json_is_object(v)){
        v = cxJsonGetGlobalJson(v);
    }
    if(json_is_string(v)){
        return atof(json_string_value(v));
    }
    return json_number_value(v);
}

static cxLong jsonToLong(json_t *v,cxLong dv)
{
    CX_RETURN(v == NULL,dv);
    if(json_is_object(v)){
        v = cxJsonGetGlobalJson(v);
    }
    if(json_is_string(v)){
        return atol(json_string_value(v));
    }
    return (cxLong)json_integer_value(v);
}

static cxBool jsonToBool(json_t *v,cxBool dv)
{
    CX_RETURN(v == NULL,dv);
    if(json_is_object(v)){
        v = cxJsonGetGlobalJson(v);
    }
    if(json_is_string(v)){
        return strcasecmp(json_string_value(v), "true") == 0;
    }
    return json_is_true(v);
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
    json_t *v = CX_JSON_PTR(json);
    return jsonToArray(v);
}

cxJson cxJsonToObject(cxJson json)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = CX_JSON_PTR(json);
    return jsonToObject(v);
}

cxInt cxJsonToInt(cxJson json,cxInt dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = CX_JSON_PTR(json);
    return jsonToInt(v, dv);
}

cxConstChars cxJsonToConstChars(cxJson json)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = CX_JSON_PTR(json);
    return jsonToConstChars(v);
}

cxString cxJsonToString(cxJson json)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = CX_JSON_PTR(json);
    return jsonToString(v);
}

cxDouble cxJsonToDouble(cxJson json,cxDouble dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = CX_JSON_PTR(json);
    return jsonToDouble(v, dv);
}

cxLong cxJsonToLong(cxJson json,cxLong dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = CX_JSON_PTR(json);
    return jsonToLong(v, dv);
}

cxBool cxJsonToBool(cxJson json,cxBool dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = CX_JSON_PTR(json);
    return jsonToBool(v, dv);
}

cxInt cxJsonIntAt(cxJson json,cxInt idx,cxInt dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    return jsonToInt(v, dv);
}

cxConstChars cxJsonConstCharsAt(cxJson json,cxInt idx)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    return jsonToConstChars(v);
}

cxString cxJsonStringAt(cxJson json,cxInt idx)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    return jsonToString(v);
}

cxBool cxJsonBoolAt(cxJson json,cxInt idx,cxBool dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    return jsonToBool(v, dv);
}

cxDouble cxJsonDoubleAt(cxJson json,cxInt idx,cxDouble dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    return jsonToDouble(v, dv);
}

cxLong cxJsonLongAt(cxJson json,cxInt idx,cxLong dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    return jsonToLong(v, dv);
}

cxJson cxJsonArrayAt(cxJson json,cxInt idx)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = json_array_get(CX_JSON_PTR(json), idx);
    return jsonToArray(v);
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
    return jsonToObject(v);
}

cxJson cxJsonArray(cxJson json,cxConstChars key)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    return jsonToArray(v);
}

cxJson cxJsonObject(cxJson json,cxConstChars key)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    return jsonToObject(v);
}

cxBool cxJsonBool(cxJson json,cxConstChars key,cxBool dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    return jsonToBool(v, dv);
}

cxConstChars cxJsonConstChars(cxJson json,cxConstChars key)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    return jsonToConstChars(v);
}

cxString cxJsonString(cxJson json,cxConstChars key)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    return jsonToString(v);
}

cxDouble cxJsonDouble(cxJson json,cxConstChars key,cxDouble dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    return jsonToDouble(v, dv);
}

cxLong cxJsonLong(cxJson json,cxConstChars key,cxLong dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    return jsonToLong(v, dv);
}

cxInt cxJsonInt(cxJson json,cxConstChars key,cxInt dv)
{
    CX_ASSERT(json != NULL, "json error");
    json_t *v = cxJsonGetJson(json, key);
    return jsonToInt(v, dv);
}











