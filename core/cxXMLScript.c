//
//  cxXMLScript.c
//  cxEngine
//
//  Created by xuhua on 10/14/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <textures/cxTextureFactory.h>
#include "cxXMLScript.h"
#include "cxHash.h"
#include "cxEventArg.h"
#include "cxEngine.h"
#include "cxRegex.h"
#include "cxAutoPool.h"
#include "cxViewRoot.h"

CX_OBJECT_INIT(cxXMLReader, cxObject)
{
    
}
CX_OBJECT_FREE(cxXMLReader, cxObject)
{
    if(this->reader != NULL){
        xmlTextReaderClose(this->reader);
        xmlFreeTextReader(this->reader);
    }
}
CX_OBJECT_TERM(cxXMLReader, cxObject)

CX_OBJECT_INIT(cxTextureAttr, cxObject)
{
    this->box = cxBoxTex2fDefault();
}
CX_OBJECT_FREE(cxTextureAttr, cxObject)
{
    CX_RELEASE(this->texture);
}
CX_OBJECT_TERM(cxTextureAttr, cxObject)

CX_OBJECT_INIT(cxXMLScript, cxObject)
{
}
CX_OBJECT_FREE(cxXMLScript, cxObject)
{
    CX_RELEASE(this->bytes);
}
CX_OBJECT_TERM(cxXMLScript, cxObject)

static cxBool cxXMLAttrRunLuaFunc(cxReaderAttrInfo *info,cxJson json)
{
    cxBool ret = false;
    if(lua_iscfunction(gL, -1) || lua_isfunction(gL, -1)){
        if(!cxJsonPush(json))lua_pushlightuserdata(gL, info);
        ret = (lua_pcall(gL, 1, 1, 0) == 0);
        lua_remove(gL, -2);
    }else{
        lua_pop(gL, 1);
    }
    return ret;
}

cxBool cxXMLBindObject(cxReaderAttrInfo *info)
{
    cxConstChars bind = cxXMLAttr(info->reader,"cxObject.bind");
    CX_RETURN(bind == NULL, false);
    cxRegex regex = cxRegexCreate("^(.+?)\\.(.+?)\\((.*?)\\)$", UTF8(bind), 0);
    if(!cxRegexNext(regex)){
        return false;
    }
    cxString jsontxt = cxRegexMatch(regex, 3);
    cxStringReplace(jsontxt, '\'', '"');
    cxJson json = cxJsonCreate(jsontxt);
    cxConstChars className = cxStringBody(cxRegexMatch(regex, 1));
    cxConstChars funcName = cxStringBody(cxRegexMatch(regex, 2));
    if(className == NULL || funcName == NULL){
        return false;
    }
    CX_ASSERT(info->object != NULL, "object error");
    lua_getglobal(gL, className);
    if(!lua_istable(gL, -1)){
        lua_pop(gL, 1);
        CX_ERROR("bind class %s not find",className);
        return false;
    }
    lua_getfield(gL, -1, funcName);
    if(!lua_isfunction(gL, -1)){
        lua_pop(gL, 2);
        CX_ERROR("bind method %s not find",funcName);
        return false;
    }
    lua_pushlightuserdata(gL, info->object);
    cxInt an = cxJsonPush(json) ? 2 : 1;
    if(lua_pcall(gL, an, 1, 0) != 0){
        CX_ERROR("run bind method %s.%s failed",className,funcName);
        lua_pop(gL, 2);
        return false;
    }
    if(!lua_istable(gL, -1)){
        lua_pop(gL, 2);
        CX_ERROR("bind func must return table");
        return false;
    }
    cxObject obj = info->object;
    obj->bind = lua_ref(gL, true);
    lua_pop(gL, 1);
    return obj->bind > 0;
}

static cxBool cxXMLAttrRunFunc(cxConstChars value,cxReaderAttrInfo *info)
{
    CX_ASSERT(value != NULL, "args error");
    cxBool ret = false;
    cxRegex regex = cxRegexCreate("^(.+?)\\((.*?)\\)$", UTF8(value), 0);
    if(!cxRegexNext(regex)){
        return ret;
    }
    cxString jsontxt = cxRegexMatch(regex, 2);
    cxStringReplace(jsontxt, '\'', '"');
    cxJson json = cxJsonCreate(jsontxt);
    cxConstChars funcName = cxStringBody(cxRegexMatch(regex, 1));
    cxViewRoot root = info->root;
    //find curr bind
from_curr:
    if(cxObjectBind(info->object) > 0){
        lua_getref(gL, cxObjectBind(info->object));
        if(!lua_istable(gL, -1)){
            lua_pop(gL, 1);
            goto from_root;
        }
        lua_getfield(gL, -1, funcName);
        ret = cxXMLAttrRunLuaFunc(info,json);
        lua_remove(gL, -2);
    }
    CX_RETURN(ret,ret);
    //find root bind
from_root:
    if(cxObjectBind(root) > 0){
        lua_getref(gL, cxObjectBind(root));
        if(!lua_istable(gL, -1)){
            lua_pop(gL, 1);
            goto from_global;
        }
        lua_getfield(gL, -1, funcName);
        ret = cxXMLAttrRunLuaFunc(info,json);
        lua_remove(gL, -2);
    }
    CX_RETURN(ret,ret);
    //find global func
from_global:
    lua_getglobal(gL, funcName);
    ret = cxXMLAttrRunLuaFunc(info,json);
    return ret;
}

static cxNumber cxXMLReaderNumber(cxConstChars svalue,cxReaderAttrInfo *info)
{
    if(!cxXMLAttrRunFunc(svalue,info)){
        return NULL;
    }
    return CX_LUA_GET_PTR(-1);
}

cxConstChars cxXMLAttr(xmlTextReaderPtr reader,cxConstChars name)
{
    xmlChar *txt = xmlTextReaderGetAttribute(reader, BAD_CAST(name));
    CX_RETURN(txt == NULL || xmlStrlen(txt) == 0, NULL);
    return cxStringBody(cxStringAttachChars((cxChar *)txt));
}

static void cxPrepareReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxBool *error = arg;
    *error = true;
    CX_ERROR("%s",msg);
}

static cxString cxPrepareReplaceTemplateVar(cxRegex regex,cxAny arg)
{
    xmlTextReaderPtr reader = arg;
    cxString value = cxRegexMatch(regex, 0);
    cxString attrName = cxRegexMatch(regex, 1);
    cxConstChars var = cxXMLAttr(reader,cxStringBody(attrName));
    CX_RETURN(var == NULL, value);
    return cxStringConstChars(var);
}

static cxString cxPrepareReplaceTemplate(cxRegex regex,cxAny arg)
{
    static cxConstChars sregex = "\\$\\{(.*?)\\}";
    cxBool error = false;
    cxString input = cxRegexMatch(regex, 0);
    CX_ASSERT(input != NULL, "get group 0 failed");
    cxString ret = NULL;
    xmlTextReaderPtr reader = xmlReaderForMemory(cxStringBody(input), cxStringLength(input), NULL, "UTF-8", 0);
    cxReaderAttrInfo *info = cxReaderAttrInfoMake(reader, NULL, NULL);
    xmlTextReaderSetErrorHandler(reader, cxPrepareReaderError, &error);
    while(xmlTextReaderRead(reader) && !error){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        if(!ELEMENT_IS_TYPE(cxTemplate)){
            continue;
        }
        //
        cxBool cond = cxXMLReadBoolAttr(info, "cond", true);
        if(!cond){
            continue;
        }
        //
        cxConstChars src = cxXMLAttr(reader,"src");
        if(src != NULL){
            cxXMLScript xml = cxEngineGetXMLScript(src);
            CX_ASSERT(xml != NULL, "get xml %s template failed", src);
            
            //process Template's Template
            cxString data = cxXMLReaderPrepareTemplate(xml->bytes);
            CX_ASSERT(data != NULL, "prepare xml error");
            
            cxRegex regex = cxRegexCreate(sregex, data, 0);
            ret = cxRegexReplace(regex, cxPrepareReplaceTemplateVar, reader);
            break;
        }
        //
        cxConstChars url = cxXMLAttr(reader,"url");
        if(url != NULL){
            cxTypes types = cxEngineDataSet(url);
            CX_ASSERT(types != NULL && cxTypesIsType(types, cxTypesString), "get url %s data failed", url);
            
            cxString data = cxXMLReaderPrepareTemplate(types->any);
            CX_ASSERT(data != NULL, "prepare xml error");
            
            cxRegex regex = cxRegexCreate(sregex, data, 0);
            ret = cxRegexReplace(regex, cxPrepareReplaceTemplateVar, reader);
            break;
        }
    }
    xmlTextReaderClose(reader);
    return ret;
}

cxString cxXMLReaderPrepareTemplate(cxString code)
{
    //replace cxTemplate
    cxRegex regex = cxRegexCreate("<cxTemplate\\s.*?/>", code, 0);
    CX_ASSERT(regex != NULL, "regex error");
    cxString ret = cxRegexReplace(regex, cxPrepareReplaceTemplate, NULL);
    if(ret == NULL){
        ret = code;
    }
    return ret;
}

xmlTextReaderPtr cxXMLReaderForString(cxString code,xmlTextReaderErrorFunc error,cxAny arg)
{
    code = cxXMLReaderPrepareTemplate(code);
    CX_ASSERT(code != NULL, "xml perpare failed");
    cxXMLReader this = CX_CREATE(cxXMLReader);
    this->reader = xmlReaderForMemory(cxStringBody(code), cxStringLength(code), NULL, "UTF-8", 0);
    if(error != NULL){
        xmlTextReaderSetErrorHandler(this->reader, error, arg);
    }
    return this->reader;
}

xmlTextReaderPtr cxXMLReaderForScript(cxXMLScript this,xmlTextReaderErrorFunc error,cxAny arg)
{
    return cxXMLReaderForString(this->bytes,error,arg);
}

cxInt cxReadFloats(cxConstChars ptr,cxFloat *values)
{
    CX_RETURN(ptr == NULL || values == NULL, 0);
    cxInt len = (cxInt)strlen(ptr) + 1;
    cxInt argLen = 0;
    cxChar arg[16]={0};
    cxInt c = 0;
    CX_RETURN(len < 2, 0);
    for(int i=0; i < len; i++){
        if(ptr[i] == ',' || ptr[i] == '\0'){
            arg[argLen] = '\0';
            (*values++) = atof(arg);
            c++;
            argLen = 0;
        }else{
            arg[argLen++] = ptr[i];
        }
    }
    return c;
}

cxInt cxReadInts(cxConstChars ptr,cxInt *values)
{
    CX_RETURN(ptr == NULL || values == NULL, 0);
    cxInt len = (cxInt)strlen(ptr) + 1;
    cxInt argLen = 0;
    cxChar arg[16]={0};
    cxInt c = 0;
    CX_RETURN(len < 2, 0);
    for(int i=0; i < len; i++){
        if(ptr[i] == ',' || ptr[i] == '\0'){
            arg[argLen] = '\0';
            c++;
            (*values++) = atoi(arg);
            argLen = 0;
        }else{
            arg[argLen++] = ptr[i];
        }
    }
    return c;
}

cxString cxXMLReaderReadOuterXml(xmlTextReaderPtr reader)
{
    xmlChar *xml = xmlTextReaderReadOuterXml(reader);
    cxString rv = NULL;
    if(xml != NULL){
        rv = cxStringConstChars((cxConstChars)xml);
    }
    xmlFree(xml);
    return rv;
}

cxConstChars cxXMLReadElementName(xmlTextReaderPtr reader)
{
    return (cxConstChars)xmlTextReaderConstName(reader);
}

cxBool cxXMLHasAttr(xmlTextReaderPtr reader,cxConstChars name)
{
    cxConstChars s = cxXMLAttr(reader,name);
    return s != NULL;
}

cxString cxXMLReadString(xmlTextReaderPtr reader)
{
    xmlChar *str = xmlTextReaderReadString(reader);
    return cxStringAttachChars((cxChar *)str);
}

static cxEventItem cxXMLFindEvent(cxEventArg args,cxBool ref)
{
    cxEventItem event = NULL;
    if(lua_isfunction(gL, -1)){
        event = CX_CREATE(cxEventItem);
        args->ref = lua_ref(gL, true);
        event->func = cxObjectLuaEventFunc;
        CX_ASSERT(args->ref > 0,"get ref error");
        CX_RETAIN_SWAP(event->arg, args);
    }else{
        lua_pop(gL, 1);
    }
    if(ref){
        lua_pop(gL, 1);
    }
    return event;
}
//method1({json});
cxEventItem cxXMLReadEvent(cxReaderAttrInfo *info, cxConstChars name)
{
    cxConstChars svalue = cxXMLAttr(info->reader, name);
    CX_RETURN(svalue == NULL, NULL);
    cxRegex regex = cxRegexCreate("^(.+?)\\((.*?)\\)$", UTF8(svalue), 0);
    if(!cxRegexNext(regex)){
        return NULL;
    }
    cxString argsCode = cxRegexMatch(regex, 2);
    cxStringReplace(argsCode, '\'', '"');
    cxConstChars funcName = cxStringBody(cxRegexMatch(regex, 1));
    cxEventItem event = NULL;
    cxEventArg args = cxStringLength(argsCode) > 0 ? cxEventArgCreate(cxStringBody(argsCode)) : CX_CREATE(cxEventArg);
    cxConstType type = cxObjectType(info->object);
    //from object bind
from_curr:
    if(cxObjectBind(info->object) <= 0){
        goto from_root;
    }
    lua_getref(gL, cxObjectBind(info->object));
    if(!lua_istable(gL, -1)){
        lua_pop(gL, 1);
        goto from_root;
    }
    lua_getfield(gL, -1, funcName);
    event = cxXMLFindEvent(args, true);
    CX_RETURN(event != NULL, event);
    //from root bind
from_root:
    if(cxObjectBind(info->root) <= 0){
        goto from_type_table;
    }
    lua_getref(gL, cxObjectBind(info->root));
    if(!lua_istable(gL, -1)){
        lua_pop(gL, 1);
        goto from_type_table;
    }
    lua_getfield(gL, -1, funcName);
    event = cxXMLFindEvent(args, true);
    CX_RETURN(event != NULL, event);
from_type_table:
    //from type table find
    lua_getglobal(gL, type);
    if(!lua_istable(gL, -1)){
        lua_pop(gL, 1);
        goto from_global;
    }
    lua_getfield(gL, -1, funcName);
    event = cxXMLFindEvent(args, true);
    CX_RETURN(event != NULL, event);
from_global:
    //from global table type
    lua_getglobal(gL, funcName);
    event = cxXMLFindEvent(args, false);
    return event;
}

cxAny cxXMLReadTypesAttr(cxReaderAttrInfo *info, cxConstChars name)
{
    cxTypes rv = NULL;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    if(cxXMLAttrRunFunc(svalue,info)) {
        return CX_LUA_GET_PTR(-1);
    }
    return rv;
}

cxFloat cxXMLReadFloatAttr(cxReaderAttrInfo *info, cxConstChars name,cxFloat value)
{
    cxFloat rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeFloat){
        rv = cxNumberToFloat(num);
    }else{
        rv = atof(svalue);
    }
    return rv;
}

cxAssist4f cxXMLReadAssist4fAttr(cxReaderAttrInfo *info, cxConstChars name,cxAssist4f value)
{
    cxAssist4f rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeAssist4f){
        rv = cxNumberToAssist4f(num);
    }else{
        cxReadFloats(svalue, &rv.v1);
    }
    return rv;
}

cxColor4f cxXMLReadColor4fAttr(cxReaderAttrInfo *info, cxConstChars name,cxColor4f value)
{
    cxColor4f rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeColor4f){
        rv = cxNumberToColor4f(num);
    }else{
        cxReadFloats(svalue, &rv.r);
    }
    return rv;
}

cxVec2f cxXMLReadVec2fAttr(cxReaderAttrInfo *info,cxConstChars name,cxVec2f value)
{
    cxVec2f rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeVec2f){
        rv = cxNumberToVec2f(num);
    }else{
        cxReadFloats(svalue, &rv.x);
    }
    return rv;
}

cxSize2f cxXMLReadSize2fAttr(cxReaderAttrInfo *info,cxConstChars name,cxSize2f value)
{
    cxSize2f rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeSize2f){
        rv = cxNumberToSize2f(num);
    }else{
        cxReadFloats(svalue, &rv.w);
    }
    return rv;
}

cxVec3f cxXMLReadVec3fAttr(cxReaderAttrInfo *info,cxConstChars name,cxVec3f value)
{
    cxVec3f rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeVec3f){
        rv = cxNumberToVec3f(num);
    }else{
        cxReadFloats(svalue, &rv.x);
    }
    return rv;
}

cxFloatRange cxXMLReadFloatRangeAttr(cxReaderAttrInfo *info,cxConstChars name,cxFloatRange value)
{
    cxFloatRange rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeFloatRange){
        rv = cxNumberToFloatRange(num);
    }else{
        cxReadFloats(svalue, &rv.v);
    }
    return rv;
}

cxVec2fRange cxXMLReadVec2fRangeAttr(cxReaderAttrInfo *info,cxConstChars name,cxVec2fRange value)
{
    cxVec2fRange rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeVec2fRange){
        rv = cxNumberToVec2fRange(num);
    }else{
        cxReadFloats(svalue, &rv.v.x);
    }
    return rv;
}

cxColor4fRange cxXMLReadColor4fRangeAttr(cxReaderAttrInfo *info,cxConstChars name,cxColor4fRange value)
{
    cxColor4fRange rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeColor4fRange){
        rv = cxNumberToColor4fRange(num);
    }else{
        cxReadFloats(svalue, &rv.v.r);
    }
    return rv;
}

cxVec2i cxXMLReadVec2iAttr(cxReaderAttrInfo *info, cxConstChars name,cxVec2i value)
{
    cxVec2i rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeVec2i){
        rv = cxNumberToVec2i(num);
    }else{
        cxReadInts(svalue, &rv.x);
    }
    return rv;
}

cxBox4i cxXMLReadBox4iAttr(cxReaderAttrInfo *info, cxConstChars name,cxBox4i value)
{
    cxBox4i rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeBox4i){
        rv = cxNumberToBox4i(num);
    }else{
        cxReadInts(svalue, &rv.l);
    }
    return rv;
}

cxBox4f cxXMLReadBox4fAttr(cxReaderAttrInfo *info, cxConstChars name,cxBox4f value)
{
    cxBox4f rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeBox4f){
        rv = cxNumberToBox4f(num);
    }else{
        cxReadFloats(svalue, &rv.l);
    }
    return rv;
}

cxTextureAttr cxXMLReadTextureAttr(cxReaderAttrInfo *info,cxConstChars name)
{
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, NULL);
    cxTextureAttr rv = NULL;
    if(cxXMLAttrRunFunc(svalue,info)){
        rv = CX_LUA_GET_PTR(-1);
    }
    if(rv != NULL && cxObjectIsType(rv, cxTextureAttrTypeName)){
        return rv;
    }
    rv = CX_CREATE(cxTextureAttr);
    cxUrlPath path = cxUrlPathParse(svalue);
    cxTexture texture = NULL;
    if(path->count > 0){
        texture = cxTextureFactoryLoadFile(path->path);
        rv->size = texture->size;
        CX_RETAIN_SWAP(rv->texture, texture);
    }
    if(path->count > 1 && texture != NULL){
        rv->box = cxTextureBox(texture, path->key);
        rv->size = cxTextureSize(texture, path->key);
    }
    return rv;
}

cxInt cxXMLReadIntAttr(cxReaderAttrInfo *info,cxConstChars name,cxInt value)
{
    cxInt rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeInt){
        rv = cxNumberToInt(num);
    }else{
        rv = atoi(svalue);
    }
    return rv;
}

cxUInt cxXMLReadUIntAttr(cxReaderAttrInfo *info, cxConstChars name,cxUInt value)
{
    cxUInt rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeUInt){
        rv = cxNumberToUInt(num);
    }else{
        rv = atoi(svalue);
    }
    return rv;
}

cxString cxXMLReadStringAttr(cxReaderAttrInfo *info,cxConstChars name)
{
    cxConstChars value = cxXMLAttr(info->reader,name);
    CX_RETURN(value == NULL, NULL);
    if(cxXMLAttrRunFunc(value,info) && lua_isuserdata(gL, -1)) {
        return CX_LUA_GET_PTR(-1);
    }
    return cxStringConstChars(value);
}

cxBool cxXMLReadBoolAttr(cxReaderAttrInfo *info,cxConstChars name,cxBool value)
{
    cxBool rv = value;
    cxConstChars svalue = cxXMLAttr(info->reader,name);
    CX_RETURN(svalue == NULL, rv);
    cxNumber num = cxXMLReaderNumber(svalue, info);
    if(num != NULL && num->type == cxNumberTypeBool){
        rv = cxNumberToBool(num);
    }else{
        rv = cxConstCharsEqu(svalue, "true");
    }
    return rv;
}








