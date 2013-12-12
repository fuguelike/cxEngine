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
#include "cxEventItem.h"
#include "cxEngine.h"
#include "cxRegex.h"
#include "cxAutoPool.h"

CX_OBJECT_INIT(cxXMLReader, cxObject)
{
    
}
CX_OBJECT_FREE(cxXMLReader, cxObject)
{
    if(this->reader != NULL){
        xmlTextReaderClose(this->reader);
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

#define PARSE_FUNCTION(vptr,fname,farg)                         \
cxConstChars ptr = vptr;                                        \
cxInt len = (cxInt)strlen(ptr) + 1;                             \
cxInt lpos = 0;                                                 \
cxInt rpos = 0;                                                 \
for(cxInt i=0; i < len; i++){                                   \
    if(ptr[i] == '('){                                          \
        lpos = i;                                               \
    }                                                           \
    if(ptr[i] == ')'){                                          \
        rpos = i;                                               \
    }                                                           \
}                                                               \
cxBool isFunc = (lpos > 0 && rpos > 0);                         \
cxChar fname[len];                                              \
cxChar farg[len];                                               \
if(isFunc){                                                     \
    int len = rpos - lpos - 1;                                  \
    memcpy(farg, ptr + lpos + 1, len);                          \
    farg[len] = '\0';                                           \
    for(int i=0; i < len;i++){                                  \
        if(farg[i] == '\''){                                    \
            farg[i] = '\"';                                     \
        }                                                       \
    }                                                           \
    len = lpos;                                                 \
    memcpy(fname, ptr, len);                                    \
    fname[len] = '\0';                                          \
}else{                                                          \
    memcpy(fname, ptr, len);                                    \
}


#define GET_FUNCTION_ITEM(item)                                 \
cxFuncItem item = NULL;                                         \
if(functions != NULL){                                          \
    item = cxHashGet(functions, cxHashStrKey(fName));           \
}                                                               \
if(item == NULL){                                               \
    item = cxEngineGetFunc(fName);                              \
}                                                               \
CX_ASSERT(item != NULL, "function %s not eixsts",fName);


#define GET_EVENT_ITEM(event)                                   \
cxEventItem event = NULL;                                       \
if(events != NULL){                                             \
    event = cxHashGet(events, cxHashStrKey(eName));             \
}                                                               \
if(event == NULL){                                              \
    event = cxEngineGetEvent(eName);                            \
}

#define CHECK_NUMBER_TYPE(num,t)                                                        \
CX_ASSERT(cxObjectIsType(num, cxNumberTypeName), "%s must return cxNumber type",fName); \
CX_ASSERT(num->type == t, "%s must return "#t" number value",fName);                    \

CX_OBJECT_INIT(cxXMLScript, cxObject)
{
}
CX_OBJECT_FREE(cxXMLScript, cxObject)
{
    CX_RELEASE(this->bytes);
}
CX_OBJECT_TERM(cxXMLScript, cxObject)

cxConstChars cxXMLAttrAuto(xmlTextReaderPtr reader,cxConstChars name)
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
    cxConstChars var = cxXMLAttr(cxStringBody(attrName));
    CX_RETURN(var == NULL, value);
    return cxStringConstChars(var);
}

static cxString cxPrepareReplaceTemplate(cxRegex regex,cxAny arg)
{
    cxConstChars sregex = "\\$\\((.*?)\\)";
    cxBool error = false;
    cxString input = cxRegexMatch(regex, 0);
    cxString ret = NULL;
    xmlTextReaderPtr reader = xmlReaderForMemory(cxStringBody(input), cxStringLength(input), NULL, "UTF-8", 0);
    xmlTextReaderSetErrorHandler(reader, cxPrepareReaderError, &error);
    while(xmlTextReaderRead(reader) && !error){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        if(!ELEMENT_IS_TYPE(cxTemplate)){
            continue;
        }
        cxConstChars src = cxXMLAttr("src");
        if(src != NULL){
            cxString data = cxAssertsData(src);
            CX_ASSERT(data != NULL, "get src %s data failed", src);
            cxRegex regex = cxRegexCreate(sregex, data, 0);
            ret = cxRegexReplace(regex, cxPrepareReplaceTemplateVar, reader);
            break;
        }
        cxConstChars url = cxXMLAttr("url");
        if(url != NULL){
            cxTypes types = cxEngineDataSet(url);
            CX_ASSERT(types != NULL && cxTypesIsType(types, cxTypesString), "get url %s data failed", url);
            cxRegex regex = cxRegexCreate(sregex, types->assist, 0);
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

//mul event use ';' split
static cxEventItem cxXMLReadEventByValue(cxHash events,cxConstChars value)
{
    cxConstChars eventName = value;
    PARSE_FUNCTION(eventName, eName, eArg);
    GET_EVENT_ITEM(event);
    //set event arg
    if(isFunc && event != NULL){
        CX_RETAIN_SWAP(event->arg, cxEventArgCreate(eArg));
    }
    return event;
}

//method1({json});method2({json})
cxArray cxXMLReadEvent(cxHash events,cxConstChars name,xmlTextReaderPtr reader)
{
    //xml view event event(arg)
    cxConstChars eventName = cxXMLAttr(name);
    CX_RETURN(eventName == NULL,NULL);
    cxArray ret = CX_CREATE(cxArray);
    cxArray list = cxStringSplit(cxStringConstChars(eventName), ";");
    CX_ARRAY_FOREACH(list, e){
        cxString item = cxArrayObject(e);
        if(item == NULL){
            continue;
        }
        cxEventItem event = cxXMLReadEventByValue(events, cxStringBody(item));
        if(event == NULL){
            continue;
        }
        cxArrayAppend(ret, event);
    }
    return ret;
}

cxFloat cxXMLReadFloatAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxFloat value)
{
    cxFloat rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberFloat(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeFloat);
        rv = cxNumberToFloat(num);
    }else{
        rv = atof(svalue);
    }
    return rv;
}

cxAssist4f cxXMLReadAssist4fAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxAssist4f value)
{
    cxAssist4f rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberAssist4f(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeAssist4f);
        rv = cxNumberToAssist4f(num);
    }else{
        cxReadFloats(svalue, &rv.v1);
    }
    return rv;
}

cxColor4f cxXMLReadColor4fAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxColor4f value)
{
    cxColor4f rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberColor4f(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeColor4f);
        rv = cxNumberToColor4f(num);
    }else{
        cxReadFloats(svalue, &rv.r);
    }
    return rv;
}

cxVec2f cxXMLReadVec2fAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxVec2f value)
{
    cxVec2f rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberVec2f(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeVec2f);
        rv = cxNumberToVec2f(num);
    }else{
        cxReadFloats(svalue, &rv.x);
    }
    return rv;
}

cxSize2f cxXMLReadSize2fAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxSize2f value)
{
    cxSize2f rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberSize2f(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeSize2f);
        rv = cxNumberToSize2f(num);
    }else{
        cxReadFloats(svalue, &rv.w);
    }
    return rv;
}

cxVec3f cxXMLReadVec3fAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxVec3f value)
{
    cxVec3f rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberVec3f(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeVec3f);
        rv = cxNumberToVec3f(num);
    }else{
        cxReadFloats(svalue, &rv.x);
    }
    return rv;
}

cxFloatRange cxXMLReadFloatRangeAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxFloatRange value)
{
    cxFloatRange rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberFloatRange(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeFloatRange);
        rv = cxNumberToFloatRange(num);
    }else{
        cxReadFloats(svalue, &rv.v);
    }
    return rv;
}

cxVec2fRange cxXMLReadVec2fRangeAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxVec2fRange value)
{
    cxVec2fRange rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberVec2fRange(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeVec2fRange);
        rv = cxNumberToVec2fRange(num);
    }else{
        cxReadFloats(svalue, &rv.v.x);
    }
    return rv;
}

cxColor4fRange cxXMLReadColor4fRangeAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxColor4fRange value)
{
    cxColor4fRange rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberColor4fRange(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeColor4fRange);
        rv = cxNumberToColor4fRange(num);
    }else{
        cxReadFloats(svalue, &rv.v.r);
    }
    return rv;
}

cxVec2i cxXMLReadVec2iAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxVec2i value)
{
    cxVec2i rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberVec2i(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeVec2i);
        rv = cxNumberToVec2i(num);
    }else{
        cxReadInts(svalue, &rv.x);
    }
    return rv;
}

cxBox4i cxXMLReadBox4iAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxBox4i value)
{
    cxBox4i rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberBox4i(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeBox4i);
        rv = cxNumberToBox4i(num);
    }else{
        cxReadInts(svalue, &rv.l);
    }
    return rv;
}

cxBox4f cxXMLReadBox4fAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxBox4f value)
{
    cxBox4f rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberBox4f(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeBox4f);
        rv = cxNumberToBox4f(num);
    }else{
        cxReadFloats(svalue, &rv.l);
    }
    return rv;
}

cxTextureAttr cxXMLReadTextureAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name)
{
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, NULL);
    cxTextureAttr rv = CX_CREATE(cxTextureAttr);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreate(fArg);
        rv = item->func(args);
    }else{
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
    }
    return rv;
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

cxConstChars cxXMLReadElementName(xmlTextReaderPtr reader)
{
    return (cxConstChars)xmlTextReaderConstName(reader);
}

cxBool cxXMLHasAttr(xmlTextReaderPtr reader,cxConstChars name)
{
    cxConstChars s = cxXMLAttr(name);
    return s != NULL;
}

cxString cxXMLReadString(xmlTextReaderPtr reader)
{
    xmlChar *str = xmlTextReaderReadString(reader);
    return cxStringAttachChars((cxChar *)str);
}

cxInt cxXMLReadIntAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxInt value)
{
    cxInt rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberInt(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeInt);
        rv = cxNumberToInt(num);
    }else{
        rv = atoi(svalue);
    }
    return rv;
}

cxUInt cxXMLReadUIntAttr(xmlTextReaderPtr reader,cxHash functions, cxConstChars name,cxUInt value)
{
    cxUInt rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberUInt(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeUInt);
        rv = cxNumberToUInt(num);
    }else{
        rv = atol(svalue);
    }
    return rv;
}

cxString cxXMLReadStringAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name)
{
    cxString rv = NULL;
    cxConstChars value = cxXMLAttr(name);
    CX_RETURN(value == NULL, NULL);
    PARSE_FUNCTION(value, fName, fArg);
    cxFuncItem item = NULL;
    if(functions != NULL){
        item = cxHashGet(functions, cxHashStrKey(fName));
    }
    if(item == NULL){
        item = cxEngineGetFunc(fName);
    }
    if(isFunc && item != NULL){
        cxEventArg args = cxEventArgCreate(fArg);
        rv = item->func(args);
    }
    if(rv == NULL){
        rv = cxStringConstChars(value);
    }
    CX_ASSERT(cxObjectIsType(rv, cxStringTypeName), "get string error");
    return rv;
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

cxBool cxXMLReadBoolAttr(xmlTextReaderPtr reader,cxHash functions,cxConstChars name,cxBool value)
{
    cxBool rv = value;
    cxConstChars svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, rv);
    PARSE_FUNCTION(svalue, fName, fArg);
    if(isFunc){
        GET_FUNCTION_ITEM(item);
        cxEventArg args = cxEventArgCreateWithNumber(fArg,cxNumberBool(rv));
        cxNumber num = item->func(args);
        CHECK_NUMBER_TYPE(num, cxNumberTypeBool);
        rv = cxNumberToBool(num);
    }else{
        rv = cxConstCharsEqu(svalue, "true");
    }
    return rv;
}








