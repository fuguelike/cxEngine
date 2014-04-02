//
//  cxHashRoot.c
//  cxEngine
//
//  Created by xuhua on 10/19/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <streams/cxAssetsStream.h>
#include <textures/cxTextureFactory.h>
#include <views/cxAtlas.h>
#include "cxEngine.h"
#include "cxAutoPool.h"
#include "cxStack.h"
#include "cxHashRoot.h"

cxTypes cxHashRootReadString(cxHashRoot root,xmlTextReaderPtr reader)
{
    cxTypes types = cxStringTypesCreate();
    cxString bytes = NULL;
    cxConstChars  src = cxXMLAttr(reader, "src");
    if(src != NULL){
        cxStream stream = cxAssetsStreamCreate(src);
        bytes = cxStreamAllBytes(stream);
    }else if(!xmlTextReaderIsEmptyElement(reader)) {
        bytes = cxXMLReadString(reader);
    }else{
        bytes = NULL;
    }
    if(bytes == NULL){
        return NULL;
    }
    CX_RETAIN_SWAP(types->any, bytes);
    return types;
}

/*
 <cxHash id="config">
    <cxString key="name">text</cxString>
    <cxInt key="int">123</cxInt>
    <cxFloat key="float">456</cxInt>
    <cxDouble key="double">789</cxInt>
    <cxVec2f key="vec2f">12,23</cxVec2f>
    <cxVec2i key="vec2i">12,23</cxVec2i>
    <cxSize2f key="size2f">100,200</cxSize2f>
 </cxHash>
*/

static cxAny cxReadValues(cxHashRoot root,cxConstChars temp,xmlTextReaderPtr reader)
{
    cxNumberValue vm={0};
    cxInt b4[4] = {255,255,255,255};
    cxString text = cxXMLReadString(reader);
    CX_RETURN(text == NULL, NULL);
    cxConstChars value = cxStringBody(text);
    cxAny rv = NULL;
    if(ELEMENT_IS_TYPE(cxPoint) && cxReadFloats(value, &vm.vp.vertices.x) == 9){
        rv = cxNumberPoint(vm.vp);
    }else if(ELEMENT_IS_TYPE(cxBool)){
        rv = cxNumberBool(cxConstCharsEqu(value, "true"));
    }else if(ELEMENT_IS_TYPE(cxInt)){
        rv = cxNumberInt(atoi(value));
    }else if(ELEMENT_IS_TYPE(cxFloat)){
        rv = cxNumberFloat(atof(value));
    }else if (ELEMENT_IS_TYPE(cxDouble)){
        rv = cxNumberDouble(atof(value));
    }else if (ELEMENT_IS_TYPE(cxVec2f) && cxReadFloats(value, &vm.vec2f.x) == 2){
        rv = cxNumberVec2f(vm.vec2f);
    }else if (ELEMENT_IS_TYPE(cxSize2f) && cxReadFloats(value, &vm.size2f.w) == 2){
        rv = cxNumberSize2f(vm.size2f);
    }else if(ELEMENT_IS_TYPE(cxColor4f) && cxReadFloats(value, &vm.color4f.r) >= 3){
        rv = cxNumberColor4f(vm.color4f);
    }else if(ELEMENT_IS_TYPE(cxVec2i) && cxReadInts(value, &vm.vec2i.x) == 2){
        rv = cxNumberVec2i(vm.vec2i);
    }else if(ELEMENT_IS_TYPE(cxColor4b) && cxReadInts(value, b4) >= 3){
        vm.color4f.r = ((cxFloat)b4[0])/255.0f;
        vm.color4f.g = ((cxFloat)b4[1])/255.0f;
        vm.color4f.b = ((cxFloat)b4[2])/255.0f;
        vm.color4f.a = ((cxFloat)b4[3])/255.0f;
        rv = cxNumberColor4f(vm.color4f);
    }else if(ELEMENT_IS_TYPE(cxHash)){
        rv = cxHashRootReadHash(root,reader);
    }else if(ELEMENT_IS_TYPE(cxArray)){
        rv = cxHashRootReadArray(root, reader);
    }
    return rv;
}

cxTypes cxHashRootReadNumber(cxHashRoot root,cxConstChars temp,xmlTextReaderPtr reader)
{
    cxTypes types = cxNumberTypesCreate();
    cxAny obj = cxReadValues(root, temp, reader);
    if(!cxObjectIsType(obj, cxNumberTypeName)){
        return NULL;
    }
    CX_RETAIN_SWAP(types->any, obj);
    return types;
}

cxTypes cxHashRootReadHash(cxHashRoot root,xmlTextReaderPtr reader)
{
    cxTypes types = cxHashTypesCreate();
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars  key = cxXMLAttr(reader,"key");
        if(key == NULL){
            continue;
        }
        cxAny value = NULL;
        cxConstChars temp = cxXMLReadElementName(reader);
        if(ELEMENT_IS_TYPE(cxString)){
            cxTypes types = cxHashRootReadString(root, reader);
            value = types != NULL ? types->any : NULL;
        } else if(ELEMENT_IS_TYPE(cxHash)){
            cxTypes types = cxHashRootReadHash(root, reader);
            value = types != NULL ? types->any : NULL;
        } else if(ELEMENT_IS_TYPE(cxArray)){
            cxTypes types = cxHashRootReadArray(root, reader);
            value = types != NULL ? types->any : NULL;
        } else {
            value = cxReadValues(root, temp, reader);
        }
        if(value != NULL){
            cxHashSet(types->any, cxHashStrKey(key), value);
        }
    }
    return types;
}

cxTypes cxHashRootReadArray(cxHashRoot root,xmlTextReaderPtr reader)
{
    cxTypes types = cxArrayTypesCreate();
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxAny value = NULL;
        cxConstChars temp = cxXMLReadElementName(reader);
        if(ELEMENT_IS_TYPE(cxString)){
            cxTypes types = cxHashRootReadString(root, reader);
            value = types != NULL ? types->any : NULL;
        } else if(ELEMENT_IS_TYPE(cxHash)){
            cxTypes types = cxHashRootReadHash(root, reader);
            value = types != NULL ? types->any : NULL;
        } else if(ELEMENT_IS_TYPE(cxArray)){
            cxTypes types = cxHashRootReadArray(root, reader);
            value = types != NULL ? types->any : NULL;
        } else {
            value = cxReadValues(root, temp, reader);
        }
        if(value != NULL){
            cxArrayAppend(types->any, value);
        }
    }
    return types;
}

CX_OBJECT_INIT(cxHashRoot, cxObject)
{
    this->items = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxHashRoot, cxObject)
{
    CX_RELEASE(this->items);
}
CX_OBJECT_TERM(cxHashRoot, cxObject)

static void cxHashRootReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxHashRoot this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

cxHashRoot cxHashRootCreate(cxConstChars xml)
{
    cxHashRoot this = CX_CREATE(cxHashRoot);
    if(!cxHashRootLoad(this, xml)){
        return NULL;
    }
    return this;
}

cxBool cxHashRootLoad(cxAny root,cxConstChars xml)
{
    cxXMLScript script = cxEngineGetXMLScript(xml);
    if(script == NULL){
        CX_ERROR("%s script not register",xml);
        return false;
    }
    if(script->bytes == NULL){
        CX_ERROR("xml script not load bytes");
        return false;
    }
    xmlTextReaderPtr reader = cxXMLReaderForScript(script, cxHashRootReaderError, root);
    if(reader == NULL){
        CX_ERROR("create xml reader failed");
        return false;
    }
    return cxHashRootLoadWithReader(root, reader);
}

cxBool cxHashRootLoadWithReader(cxHashRoot root,xmlTextReaderPtr reader)
{
    cxBool ret = false;
    while(xmlTextReaderRead(reader)){
        if(root->isError){
            break;
        }
        CX_CONTINUE(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT);
        cxConstChars temp = cxXMLReadElementName(reader);
        if(ELEMENT_IS_TYPE(cxHashRoot)){
            ret = true;
            break;
        }
    }
    CX_RETURN(!ret,false);
    cxAutoPoolPush();
    while(xmlTextReaderRead(reader)){
        CX_CONTINUE(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT);
        cxConstChars temp = cxXMLReadElementName(reader);
        cxConstChars  sid = cxXMLAttr(reader, "id");
        if(sid == NULL){
            CX_WARN("element %s:not set id,data not save to hash table",temp);
            continue;
        }
        cxAny object = NULL;
        if(ELEMENT_IS_TYPE(cxString)){
            //cxTypesString
            object = cxHashRootReadString(root,reader);
        }else if(ELEMENT_IS_TYPE(cxHash)){
            //cxTypesHash
            object = cxHashRootReadHash(root,reader);
        }else if(ELEMENT_IS_TYPE(cxArray)){
            //cxTypesArray
            object = cxHashRootReadArray(root,reader);
        }else{
            //cxTypesNumber
            object = cxHashRootReadNumber(root, temp, reader);
        }
        if(object != NULL){
            cxHashSet(root->items, cxHashStrKey(sid), object);
        }
    }
    cxAutoPoolPop();
    return ret;
}











