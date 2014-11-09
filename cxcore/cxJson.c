//
//  cxJson.c
//  cxEngine
//
//  Created by xuhua on 11/13/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <lzma/cxLzma.h>
#include "cxNumber.h"
#include "cxJson.h"
#include "cxHash.h"

static cxString jsonAESKey = NULL;

cxJson cxJsonRead(cxConstChars src)
{
    cxChar path[512]={0};
    cxInt type = cxConstCharsTypePath(src, path);
    if(type == 0){
        return NULL;
    }
    src = path;
    if(type == 2){
        json_t *json = cxJsonLocalized(path);
        if(json != NULL && json_is_string(json))src = json_string_value((json));
    }
    return cxJsonLoader(src);
}

//if field is array
static cxBool keyIsArray(cxConstChars ckey,cxChars skey,cxInt *index)
{
    CX_ASSERT(skey != NULL && ckey != NULL, "args error");
    cxInt s = -1;
    cxInt e = -1;
    cxInt len = (cxInt)strlen(ckey);
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
//support key.key key[0].filed, max 8 level
static json_t *jsonGetJson(json_t *json,cxConstChars key)
{
    CX_ASSERT(cxConstCharsOK(key), "key error");
    cxChars ckey = allocator->strdup(key);
    cxInt num = 0;
    cxChars ckeys[8];
    cxChars src = ckey;
    ckeys[num++] = src;
    while (*src++ != '\0') {
        if(*src != '.')continue;
        CX_ASSERT(num < 8, ". opt too more");
        ckeys[num++] = src + 1;
        *src++ = '\0';
    }
    if(num == 1){
        allocator->free(ckey);
        return json_object_get(json, key);
    }
    json_t *pv = json;
    cxInt index = 0;
    cxChar skey[CX_HASH_MAX_KEY]={0};
    json_t *rv = NULL;
    for(cxInt i=0; i < num;i ++){
        cxConstChars ckey = ckeys[i];
        if(keyIsArray(ckey, skey, &index)){
            pv = json_object_get(pv, skey);
            rv = json_array_get(pv, index);
        }else{
            rv = json_object_get(pv, skey);
        }
        if(rv == NULL)break;
        pv = rv;
    }
    allocator->free(ckey);
    return rv;
}
//auto relase json_t
json_t *jsonCreateString(cxConstChars str)
{
    CX_ASSERT(str != NULL, "str error");
    cxJson this = CX_CREATE(cxJson);
    this->json = json_string(str);
    return CX_JSON_PTR(this);
}
//
static json_t *cxJsonGetJson(cxJson json,cxConstChars key)
{
    CX_ASSERT_THIS(json, cxJson);
    CX_ASSERT(cxConstCharsOK(key), "key error");
    return jsonGetJson(CX_JSON_PTR(this), key);
}

static json_t *jsonParseRegisterValue(json_t *v)
{
    if(!json_is_string(v)){                          //$ui.json?title
        return v;
    }
    cxChar key[512]={0};
    cxInt type = cxConstCharsTypePath(json_string_value(v), key);
    if(type == 0 || type == 1){
        return v;
    }
    if(type == 2){
        return cxJsonLocalized(key);
    }
    json_t *pv = NULL;
    cxLoader loader = cxLoaderTop();
    if(cxObjectInstanceOf(loader, cxLoaderTypeName)){
        pv = CX_CALL(loader, Property, CX_M(json_t *,cxConstChars),key);
    }
    if(pv != NULL){
        return pv;
    }
    pv = cxJsonProperty(key);
    if(pv != NULL){
        return pv;
    }
    return v;
}

static cxJson cxJsonParseRegisterValue(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = jsonParseRegisterValue(CX_JSON_PTR(this));
    CX_ASSERT(v != NULL, "v null");
    return cxJsonAttachCreate(v);
}

cxString cxJsonDump(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    cxChars jsonText = json_dumps(CX_JSON_PTR(this), JSON_ENCODE_ANY);
    return cxStringAttachMem(jsonText, (cxInt)strlen(jsonText));
}

void cxJsonInit()
{
    json_set_alloc_funcs((json_malloc_t)allocator->malloc, (json_free_t)allocator->free);
}

void cxJsonFree()
{

}

static cxJson jsonToAny(json_t *v)
{
    CX_RETURN(v == NULL,NULL);
    cxJson rv = CX_CREATE(cxJson);
    rv->json = json_incref(v);
    return rv;
}

static cxJson jsonToArray(json_t *v)
{
    CX_RETURN(v == NULL,NULL);
    if(!json_is_array(v)){
        return NULL;
    }
    return jsonToAny(v);
}

static cxJson jsonToObject(json_t *v)
{
    CX_RETURN(v == NULL,NULL);
    if(!json_is_object(v)){
        return NULL;
    }
    return jsonToAny(v);
}

static cxInt jsonToInt(json_t *v,cxInt dv)
{
    CX_RETURN(v == NULL,dv);
    v = jsonParseRegisterValue(v);
    if(json_is_string(v)){
        return atoi(json_string_value(v));
    }
    return (cxInt)json_number_value(v);
}

static cxConstChars jsonToConstChars(json_t *v)
{
    CX_RETURN(v == NULL, NULL);
    cxConstChars str = NULL;
    v = jsonParseRegisterValue(v);
    if(json_is_string(v)){
        str = json_string_value(v);
    }else if(json_is_integer(v)){
        str = cxConstString("%d",json_integer_value(v));
    }else if(json_is_real(v)){
        str = cxConstString("%f",json_real_value(v));
    }else if(json_is_boolean(v)){
        str = cxConstString("%s",json_is_true(v)?"true":"false");
    }
    return str;
}

static cxString jsonToString(json_t *v)
{
    CX_RETURN(v == NULL, NULL);
    cxConstChars str = jsonToConstChars(v);
    return cxConstCharsOK(str)?cxStringConstChars(str) : NULL;
}

static cxDouble jsonToDouble(json_t *v,cxDouble dv)
{
    CX_RETURN(v == NULL,dv);
    v = jsonParseRegisterValue(v);
    if(json_is_string(v)){
        return atof(json_string_value(v));
    }
    return json_number_value(v);
}

static cxLong jsonToLong(json_t *v,cxLong dv)
{
    CX_RETURN(v == NULL,dv);
    v = jsonParseRegisterValue(v);
    if(json_is_string(v)){
        return atol(json_string_value(v));
    }
    return (cxLong)json_integer_value(v);
}

static cxBool jsonToBool(json_t *v,cxBool dv)
{
    CX_RETURN(v == NULL,dv);
    v = jsonParseRegisterValue(v);
    if(json_is_string(v)){
        return strcasecmp(json_string_value(v), "true") == 0;
    }
    if(json_is_integer(v)){
        return json_integer_value(v) != 0;
    }
    return json_is_true(v);
}

CX_TYPE(cxJson, cxObject)
{
    
}
CX_INIT(cxJson, cxObject)
{
    
}
CX_FREE(cxJson, cxObject)
{
    if(this->json != NULL){
        json_decref(this->json);
    }
}
CX_TERM(cxJson, cxObject)
//

//{"vx":0,"vy":0,"rx":0,"ry":0}
cxVec2fRange cxJsonToVec2fRangle(cxJson json,cxVec2fRange dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.v.x = cxJsonDouble(this, "vx", dv.v.x);
    dv.v.y = cxJsonDouble(this, "vy", dv.v.y);
    dv.r.x = cxJsonDouble(this, "rx", dv.r.x);
    dv.r.y = cxJsonDouble(this, "ry", dv.r.y);
    return dv;
}
cxJson cxJsonVec2fRangleToJson(cxVec2fRange dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "vx", dv.v.x);
    cxJsonSetDouble(json, "vy", dv.v.y);
    cxJsonSetDouble(json, "rx", dv.r.x);
    cxJsonSetDouble(json, "ry", dv.r.y);
    return json;
}

//{"v":0,"r":0}
cxFloatRange cxJsonToFloatRangle(cxJson json,cxFloatRange dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.v = cxJsonDouble(this, "v", dv.v);
    dv.r = cxJsonDouble(this, "r", dv.r);
    return dv;
}
cxJson cxJsonFloatRangeToJson(cxFloatRange dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "v", dv.v);
    cxJsonSetDouble(json, "r", dv.r);
    return json;
}

//{"w":0,"h":0}
cxSize2f cxJsonToSize2f(cxJson json,cxSize2f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.w = cxJsonDouble(this, "w", dv.w);
    dv.h = cxJsonDouble(this, "h", dv.h);
    return dv;
}
cxJson cxJsonSize2fToJson(cxSize2f dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "w", dv.w);
    cxJsonSetDouble(json, "h", dv.h);
    return json;
}

//{"w":0,"h":0}
cxSize2i cxJsonToSize2i(cxJson json,cxSize2i dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.w = cxJsonInt(this, "w", dv.w);
    dv.h = cxJsonInt(this, "h", dv.h);
    return dv;
}
cxJson cxJsonSizeifToJson(cxSize2i dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetInt(json, "w", dv.w);
    cxJsonSetInt(json, "h", dv.h);
    return json;
}

//{"x":0,"y":0,"z":0}
cxVec3f cxJsonToVec3f(cxJson json,cxVec3f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.x = cxJsonDouble(this, "x", dv.x);
    dv.y = cxJsonDouble(this, "y", dv.y);
    dv.z = cxJsonDouble(this, "z", dv.z);
    return dv;
}
cxJson cxJsonVec3fToJson(cxVec3f dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "x", dv.x);
    cxJsonSetDouble(json, "y", dv.y);
    cxJsonSetDouble(json, "z", dv.z);
    return json;
}

//{"x":0,"y":0}
cxVec2i cxJsonToVec2i(cxJson json,cxVec2i dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.x = cxJsonInt(this, "x", dv.x);
    dv.y = cxJsonInt(this, "y", dv.y);
    return dv;
}
cxJson cxJsonVec2iToJson(cxVec2i dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetInt(json, "x", dv.x);
    cxJsonSetInt(json, "y", dv.y);
    return json;
}

//{"min":0,"max":0}
cxRange2f cxJsonToRange2f(cxJson json,cxRange2f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.min = cxJsonDouble(this, "min", dv.min);
    dv.max = cxJsonDouble(this, "max", dv.max);
    return dv;
}
cxJson cxJsonRange2fToJson(cxRange2f dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "min", dv.min);
    cxJsonSetDouble(json, "max", dv.max);
    return json;
}

//{"x":0,"y":0}
cxVec2f cxJsonToVec2f(cxJson json,cxVec2f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.x = cxJsonDouble(this, "x", dv.x);
    dv.y = cxJsonDouble(this, "y", dv.y);
    return dv;
}
cxJson cxJsonVec2fToJson(cxVec2f dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "x", dv.x);
    cxJsonSetDouble(json, "y", dv.y);
    return json;
}


//{"vr":0,"vg":0,"vb":0,"va":0,"rr":0,"rg":0,"rb":0,"ra":0}
cxColor4fRange cxJsonToColor4fRangle(cxJson json,cxColor4fRange dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.v.r = cxJsonDouble(this, "vr", dv.v.r);
    dv.v.g = cxJsonDouble(this, "vg", dv.v.g);
    dv.v.b = cxJsonDouble(this, "vb", dv.v.b);
    dv.v.a = cxJsonDouble(this, "va", dv.v.a);
    dv.r.r = cxJsonDouble(this, "rr", dv.r.r);
    dv.r.g = cxJsonDouble(this, "rg", dv.r.g);
    dv.r.b = cxJsonDouble(this, "rb", dv.r.b);
    dv.r.a = cxJsonDouble(this, "ra", dv.r.a);
    return dv;
}
cxJson cxJsonColor4fRangeToJson(cxColor4fRange dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "vr", dv.v.r);
    cxJsonSetDouble(json, "vg", dv.v.g);
    cxJsonSetDouble(json, "vb", dv.v.b);
    cxJsonSetDouble(json, "va", dv.v.a);
    cxJsonSetDouble(json, "rr", dv.r.r);
    cxJsonSetDouble(json, "rg", dv.r.g);
    cxJsonSetDouble(json, "rb", dv.r.b);
    cxJsonSetDouble(json, "ra", dv.r.a);
    return json;
}

//{"r":0,"g":0,"b":0,"a":0}
cxColor4f cxJsonToColor4f(cxJson json,cxColor4f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.r = cxJsonDouble(this, "r", dv.r);
    dv.g = cxJsonDouble(this, "g", dv.g);
    dv.b = cxJsonDouble(this, "b", dv.b);
    dv.a = cxJsonDouble(this, "a", dv.a);
    return dv;
}
cxJson cxJsonColor4fToJson(cxColor4f dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "r", dv.r);
    cxJsonSetDouble(json, "g", dv.g);
    cxJsonSetDouble(json, "b", dv.b);
    cxJsonSetDouble(json, "a", dv.a);
    return json;
}

//{"r":0,"g":0,"b":0,"a":1.0}
cxColor3f cxJsonToColor3f(cxJson json,cxColor3f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.r = cxJsonDouble(this, "r", dv.r);
    dv.g = cxJsonDouble(this, "g", dv.g);
    dv.b = cxJsonDouble(this, "b", dv.b);
    return dv;
}
cxJson cxJsonColor3fTojson(cxColor3f dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "r", dv.r);
    cxJsonSetDouble(json, "g", dv.g);
    cxJsonSetDouble(json, "b", dv.b);
    return json;
}

//{"l":0,"r":0,"t":0,"b":0}
cxBox4f cxJsonToBox4f(cxJson json,cxBox4f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.l = cxJsonDouble(this, "l", dv.l);
    dv.r = cxJsonDouble(this, "r", dv.r);
    dv.t = cxJsonDouble(this, "t", dv.t);
    dv.b = cxJsonDouble(this, "b", dv.b);
    return dv;
}
cxJson cxJsonBox4fTojson(cxBox4f dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "l", dv.l);
    cxJsonSetDouble(json, "r", dv.r);
    cxJsonSetDouble(json, "t", dv.t);
    cxJsonSetDouble(json, "b", dv.b);
    return json;
}

//{"u":0,"v":0}
cxTex2f cxJsonToTex2f(cxJson json,cxTex2f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.u = cxJsonDouble(this, "u", dv.u);
    dv.v = cxJsonDouble(this, "v", dv.v);
    return dv;
}
cxJson cxJsonTex2fToJson(cxTex2f dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetDouble(json, "u", dv.u);
    cxJsonSetDouble(json, "v", dv.v);
    return json;
}

//{"lb":{"u":0,"v":0},"rb":{"u":0,"v":0},"lt":{"u":0,"v":0},"rt":{"u":0,"v":0}}
cxBoxTex2f cxJsonToBoxTex2f(cxJson json,cxBoxTex2f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    this = cxJsonParseRegisterValue(this);
    dv.lb = cxJsonTex2f(this, "lb", dv.lb);
    dv.rb = cxJsonTex2f(this, "rb", dv.rb);
    dv.lt = cxJsonTex2f(this, "lt", dv.lt);
    dv.rt = cxJsonTex2f(this, "rt", dv.rt);
    return dv;
}
cxJson cxJsonBoxTex2fToJson(cxBoxTex2f dv)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetJson(json, "lb", cxJsonTex2fToJson(dv.lb));
    cxJsonSetJson(json, "rb", cxJsonTex2fToJson(dv.rb));
    cxJsonSetJson(json, "lt", cxJsonTex2fToJson(dv.lt));
    cxJsonSetJson(json, "rt", cxJsonTex2fToJson(dv.rt));
    return json;
}

/////////////////////////////////////////////

cxVec2fRange cxJsonVec2fRangle(cxJson json,cxConstChars key,cxVec2fRange dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToVec2fRangle(obj, dv);
}

cxFloatRange cxJsonFloatRangle(cxJson json,cxConstChars key,cxFloatRange dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToFloatRangle(obj, dv);
}

cxSize2f cxJsonSize2f(cxJson json,cxConstChars key,cxSize2f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToSize2f(obj, dv);
}

cxSize2i cxJsonSize2i(cxJson json,cxConstChars key,cxSize2i dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToSize2i(obj, dv);
}

cxVec3f cxJsonVec3f(cxJson json,cxConstChars key,cxVec3f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToVec3f(obj, dv);
}

cxVec2i cxJsonVec2i(cxJson json,cxConstChars key,cxVec2i dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToVec2i(obj, dv);
}

cxRange2f cxJsonRange2f(cxJson json,cxConstChars key,cxRange2f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToRange2f(obj, dv);
}

cxVec2f cxJsonVec2f(cxJson json,cxConstChars key,cxVec2f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToVec2f(obj, dv);
}

//{"vr":0,"vg":0,"vb":0,"va":0,"rr":0,"rg":0,"rb":0,"ra":0}
cxColor4fRange cxJsonColor4fRangle(cxJson json,cxConstChars key,cxColor4fRange dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToColor4fRangle(obj, dv);
}

//{"r":0,"g":0,"b":0,"a":0}
cxColor4f cxJsonColor4f(cxJson json,cxConstChars key,cxColor4f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToColor4f(obj, dv);
}

//{"l":0,"r":0,"t":0,"b":0}
cxBox4f cxJsonBox4f(cxJson json,cxConstChars key,cxBox4f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToBox4f(obj, dv);
}

//{"u":0,"v":0}
cxTex2f cxJsonTex2f(cxJson json,cxConstChars key,cxTex2f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToTex2f(obj, dv);
}

//{"lb":{"u":0,"v":0},"rb":{"u":0,"v":0},"lt":{"u":0,"v":0},"rt":{"u":0,"v":0}}
cxBoxTex2f cxJsonBoxTex2f(cxJson json,cxConstChars key,cxBoxTex2f dv)
{
    CX_ASSERT_THIS(json, cxJson);
    cxJson obj = cxJsonObject(this, key);
    CX_RETURN(!cxJsonIsObject(obj),dv);
    return cxJsonToBoxTex2f(obj, dv);
}

cxBool cxJsonExists(cxJson json,cxConstChars key)
{
    CX_ASSERT_THIS(json, cxJson);
    return json_object_get(CX_JSON_PTR(this), key) != NULL;
}

cxBool cxJsonIsInt(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    return json_is_integer(CX_JSON_PTR(this));
}

cxBool cxJsonIsDouble(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    return json_is_real(CX_JSON_PTR(this));
}

cxBool cxJsonIsNumber(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    return json_is_number(CX_JSON_PTR(this));
}

cxBool cxJsonIsString(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    return json_is_string(CX_JSON_PTR(this));
}

cxBool cxJsonIsBool(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    return json_is_boolean(CX_JSON_PTR(this));
}

cxBool cxJsonIsArray(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    return json_is_array(CX_JSON_PTR(this));
}

cxBool cxJsonIsObject(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    return json_is_object(CX_JSON_PTR(this));
}

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

//use json_object make json
cxJson cxJsonAttach(json_t *json)
{
    CX_ASSERT(json != NULL, "args error");
    cxJson this = CX_CREATE(cxJson);
    this->json = json;
    return this;
}

cxJson cxJsonAttachCreate(json_t *json)
{
    CX_ASSERT(json != NULL, "args error");
    cxJson this = CX_CREATE(cxJson);
    this->json = json;
    json_incref(this->json);
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
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = CX_JSON_PTR(this);
    return jsonToArray(v);
}

cxJson cxJsonToObject(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = CX_JSON_PTR(this);
    return jsonToObject(v);
}

cxInt cxJsonToInt(cxJson json,cxInt dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = CX_JSON_PTR(this);
    return jsonToInt(v, dv);
}

cxConstChars cxJsonToConstChars(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = CX_JSON_PTR(this);
    return jsonToConstChars(v);
}

cxString cxJsonToString(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = CX_JSON_PTR(this);
    return jsonToString(v);
}

cxDouble cxJsonToDouble(cxJson json,cxDouble dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = CX_JSON_PTR(this);
    return jsonToDouble(v, dv);
}

cxLong cxJsonToLong(cxJson json,cxLong dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = CX_JSON_PTR(this);
    return jsonToLong(v, dv);
}

cxBool cxJsonToBool(cxJson json,cxBool dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = CX_JSON_PTR(this);
    return jsonToBool(v, dv);
}

cxInt cxJsonIntAt(cxJson json,cxInt idx,cxInt dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = json_array_get(CX_JSON_PTR(this), idx);
    return jsonToInt(v, dv);
}

cxConstChars cxJsonConstCharsAt(cxJson json,cxInt idx)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = json_array_get(CX_JSON_PTR(this), idx);
    return jsonToConstChars(v);
}

cxString cxJsonStringAt(cxJson json,cxInt idx)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = json_array_get(CX_JSON_PTR(this), idx);
    return jsonToString(v);
}

cxBool cxJsonBoolAt(cxJson json,cxInt idx,cxBool dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = json_array_get(CX_JSON_PTR(this), idx);
    return jsonToBool(v, dv);
}

cxDouble cxJsonDoubleAt(cxJson json,cxInt idx,cxDouble dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = json_array_get(CX_JSON_PTR(this), idx);
    return jsonToDouble(v, dv);
}

cxLong cxJsonLongAt(cxJson json,cxInt idx,cxLong dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = json_array_get(CX_JSON_PTR(this), idx);
    return jsonToLong(v, dv);
}

cxJson cxJsonArrayAt(cxJson json,cxInt idx)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = json_array_get(CX_JSON_PTR(this), idx);
    return jsonToArray(v);
}

cxInt cxJsonObjectLength(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    return (cxInt)json_object_size(CX_JSON_PTR(this));
}

cxInt cxJsonArrayLength(cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    return (cxInt)json_array_size(CX_JSON_PTR(this));
}

cxJson cxJsonAnyAt(cxJson json,cxInt idx)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = json_array_get(CX_JSON_PTR(this), idx);
    return jsonToAny(v);
}

cxJson cxJsonObjectAt(cxJson json,cxInt idx)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = json_array_get(CX_JSON_PTR(this), idx);
    return jsonToObject(v);
}

cxJson cxJsonArray(cxJson json,cxConstChars key)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = cxJsonGetJson(this, key);
    return jsonToArray(v);
}

cxJson cxJsonObject(cxJson json,cxConstChars key)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = cxJsonGetJson(this, key);
    return jsonToObject(v);
}

cxBool cxJsonBool(cxJson json,cxConstChars key,cxBool dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = cxJsonGetJson(this, key);
    return jsonToBool(v, dv);
}

cxConstChars cxJsonConstChars(cxJson json,cxConstChars key)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = cxJsonGetJson(this, key);
    return jsonToConstChars(v);
}

cxString cxJsonString(cxJson json,cxConstChars key)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = cxJsonGetJson(this, key);
    return jsonToString(v);
}

cxFloat cxJsonFloat(cxJson json,cxConstChars key,cxFloat dv)
{
    return cxJsonDouble(json, key, dv);
}

cxDouble cxJsonDouble(cxJson json,cxConstChars key,cxDouble dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = cxJsonGetJson(this, key);
    return jsonToDouble(v, dv);
}

cxLong cxJsonLong(cxJson json,cxConstChars key,cxLong dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = cxJsonGetJson(this, key);
    return jsonToLong(v, dv);
}

cxInt cxJsonInt(cxJson json,cxConstChars key,cxInt dv)
{
    CX_ASSERT_THIS(json, cxJson);
    json_t *v = cxJsonGetJson(this, key);
    return jsonToInt(v, dv);
}

static void cxJsonDecodeToArray(cxArray array,cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    CX_JSON_ARRAY_EACH_BEG(this, item)
    cxAny iv = cxJsonDecode(item);
    if(iv == NULL){
        continue;
    }
    cxArrayAppend(array, iv);
    CX_JSON_ARRAY_EACH_END(this, item)
}

static void cxJsonDecodeToHash(cxHash hash,cxJson json)
{
    CX_ASSERT_THIS(json, cxJson);
    CX_JSON_OBJECT_EACH_BEG(this, item)
    cxAny iv = cxJsonDecode(item);
    if(iv == NULL){
        continue;
    }
    cxHashSet(hash, cxHashStrKey(itemKey), iv);
    CX_JSON_OBJECT_EACH_END(this, item)
}

cxAny cxJsonDecode(cxJson this)
{
    cxAny rv = NULL;
    if(cxJsonIsArray(this)){
        rv = CX_CREATE(cxArray);
        cxJsonDecodeToArray(rv, this);
    }else if(cxJsonIsObject(this)){
        rv = CX_CREATE(cxHash);
        cxJsonDecodeToHash(rv, this);
    }else if(cxJsonIsInt(this)){
        rv = cxNumberInt(cxJsonToInt(this, 0));
    }else if(cxJsonIsDouble(this)){
        rv = cxNumberDouble(cxJsonToDouble(this, 0));
    }else if(cxJsonIsString(this)){
        rv = cxJsonToString(this);
    }else if(cxJsonIsBool(this)){
        rv = cxNumberBool(cxJsonToBool(this, false));
    }else{
        json_t *jv = CX_JSON_PTR(this);
        CX_WARN("json type %d not process",json_typeof(jv));
        rv = NULL;
    }
    return rv;
}

void cxJsonSetAESKey(cxString v)
{
    CX_RETAIN_SWAP(jsonAESKey, v);
}

cxString cxJsonAESEncode(cxJson json)
{
    return cxJsonAESEncodeWithKey(json, jsonAESKey);
}

cxJson cxJsonAESDecode(cxString data)
{
    return cxJsonAESDecodeWithKey(data, jsonAESKey);
}

cxString cxJsonAESEncodeWithKey(cxJson json,cxString key)
{
    CX_ASSERT(key != NULL, "cxJson not set jsonAESKey");
    cxString data = cxJsonDump(json);
    if(data == NULL){
        CX_ERROR("json dump error");
        return NULL;
    }
    data = cxLzmaCompressed(data);
    if(data == NULL){
        CX_ERROR("compress json failed");
        return NULL;
    }
    return cxAESEncode(data, key);
}

cxJson cxJsonAESDecodeWithKey(cxString data,cxString key)
{
    CX_ASSERT(key != NULL, "cxJson not set key");
    cxInt bytes = cxStringLength(data);
    if(bytes < 32 || bytes % AES_KEY_LENGTH != 0){
        CX_ERROR("aes decode data error");
        return NULL;
    }
    cxString json = cxAESDecode(data, key);
    if(json == NULL){
        CX_ERROR("aes decode error");
        return NULL;
    }
    json = cxLzmaDecompress(json);
    if(json == NULL){
        CX_ERROR("json uncompress failed");
        return NULL;
    }
    return cxJsonCreate(json);
}

cxJson cxJsonCreateObject()
{
    cxJson this = CX_CREATE(cxJson);
    this->json = json_object();
    return this;
}

void cxJsonSetConstChars(cxJson json,cxConstChars key,cxConstChars v)
{
    CX_ASSERT(json != NULL && cxJsonIsObject(json), "json error");
    json_object_set_new(CX_JSON_PTR(json), key, json_string(v));
}

void cxJsonSetJson(cxJson json,cxConstChars key,cxJson v)
{
    CX_ASSERT(json != NULL && cxJsonIsObject(json), "json error");
    json_object_set(CX_JSON_PTR(json), key, CX_JSON_PTR(v));
}

void cxJsonSetString(cxJson json,cxConstChars key,cxString v)
{
    CX_ASSERT(cxStringOK(v), "string error");
    cxJsonSetConstChars(json, key, cxStringBody(v));
}

void cxJsonSetInt(cxJson json,cxConstChars key,cxLong v)
{
    CX_ASSERT(json != NULL && cxJsonIsObject(json), "json error");
    json_object_set_new(CX_JSON_PTR(json), key, json_integer(v));
}

void cxJsonSetDouble(cxJson json,cxConstChars key,cxDouble v)
{
    CX_ASSERT(json != NULL && cxJsonIsObject(json), "json error");
    json_object_set_new(CX_JSON_PTR(json), key, json_real(v));
}

void cxJsonSetBool(cxJson json,cxConstChars key,cxBool v)
{
    CX_ASSERT(json != NULL && cxJsonIsObject(json), "json error");
    json_object_set_new(CX_JSON_PTR(json), key, json_boolean(v));
}

cxJson cxJsonCreateArray()
{
    cxJson this = CX_CREATE(cxJson);
    this->json = json_array();
    return this;
}

void cxJsonAppendConstChars(cxJson json,cxConstChars v)
{
    CX_ASSERT(json != NULL && cxJsonIsArray(json), "json error");
    json_array_append_new(CX_JSON_PTR(json), json_string(v));
}

void cxJsonAppendString(cxJson json,cxString v)
{
    CX_ASSERT(json != NULL && cxJsonIsArray(json), "json error");
    cxJsonAppendConstChars(json, cxStringBody(v));
}

void cxJsonAppendInt(cxJson json,cxLong v)
{
    CX_ASSERT(json != NULL && cxJsonIsArray(json), "json error");
    json_array_append_new(CX_JSON_PTR(json), json_integer(v));
}

void cxJsonAppendDouble(cxJson json,cxDouble v)
{
    CX_ASSERT(json != NULL && cxJsonIsArray(json), "json error");
    json_array_append_new(CX_JSON_PTR(json), json_real(v));
}

void cxJsonAppendBool(cxJson json,cxBool v)
{
    CX_ASSERT(json != NULL && cxJsonIsArray(json), "json error");
    json_array_append_new(CX_JSON_PTR(json), json_boolean(v));
}

void cxJsonAppendJson(cxJson json,cxJson v)
{
    CX_ASSERT(json != NULL && cxJsonIsArray(json), "json error");
    json_array_append(CX_JSON_PTR(json), CX_JSON_PTR(v));
}

static void cxTypeRunObjectSetter(cxObject pobj,cxJson json)
{
    CX_ASSERT_THIS(pobj, cxObject);
    CX_JSON_OBJECT_EACH_BEG(json, item)
    cxPropertyRunSetter(this, itemKey, item);
    CX_JSON_OBJECT_EACH_END(json, item)
}

static void cxObjectSave(cxAny object,cxJson json)
{
    cxConstChars cxId = cxJsonConstChars(json, "cxId");
    CX_RETURN(cxId == NULL);
    cxLoader loader = cxLoaderTop();
    if(cxObjectInstanceOf(loader, cxLoaderTypeName)){
        CX_CALL(loader, SetItem, CX_M(void,cxConstChars,cxAny),cxId,object);
    }
}

cxAny cxJsonMakeObject(cxJson json)
{
    CX_ASSERT_TYPE(json, cxJson);
    cxAny object = NULL;
    cxJson ojson = json;
    cxJson njson = NULL;
    cxConstChars src = NULL;
    cxConstChars type = NULL;
    if(cxJsonIsString(json)){
        src = cxJsonToConstChars(json);
    }else if(cxJsonIsObject(json)){
        src = cxJsonConstChars(json, "cxSrc");
    }
    //from src get json
    if(src != NULL){
        njson = cxJsonRead(src);
        CX_ASSERT(njson != NULL, "read json failed from %s",src);
        type = cxJsonConstChars(njson, "cxType");
    }else{
        type = cxJsonConstChars(ojson, "cxType");
    }
    CX_ASSERT(type != NULL, "json cxType property null");
    object = cxTypesCreateObject(type);
    CX_ASSERT(object != NULL,"create object %s failed", type);
    //read new json property
    if(njson != NULL){
        cxTypeRunObjectSetter(object, njson);
        cxObjectSave(object, njson);
    }
    //read old json property
    if(cxJsonIsObject(ojson)){
        cxTypeRunObjectSetter(object, ojson);
        cxObjectSave(object, ojson);
    }
    return object;
}

cxAny cxJsonTocxObject(cxJson v)
{
    cxAny any = NULL;
    if(cxJsonIsInt(v)){
        any = cxNumberInt(cxJsonToInt(v, 0));
    }else if(cxJsonIsBool(v)){
        any = cxNumberBool(cxJsonToBool(v, false));
    }else if(cxJsonIsDouble(v)){
        any = cxNumberDouble(cxJsonToDouble(v, 0));
    }else if(cxJsonIsString(v)){
        any = cxStringCreate("%s",cxJsonToConstChars(v));
    }else if(cxJsonIsObject(v)){
        any = cxJsonMakeObject(v);
    }else {
        any = v;
    }
    return any;
}







