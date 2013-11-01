//
//  cxHashXML.c
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
#include "cxHashXML.h"
#include "cxDB.h"

cxTypes cxHashXMLReadAtlasBoxPoint(cxConstChars texfile,xmlTextReaderPtr reader)
{
    cxTypes types = cxAtlasBoxPointTypesCreate();
    cxTexture texture = NULL;
    cxInt index = 0;
    if(texfile != NULL){
        types->assist = cxStringAllocChars(texfile);
        texture = cxTextureFactoryLoadFile(texfile);
    }
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(!ELEMENT_IS_TYPE(item)){
            continue;
        }
        cxAtlasBoxPointType item = {0};
        
        cxChar *top = cxXMLAttr("top");
        if(top != NULL){
            item.mask |= cxViewAutoResizeTop;
            item.box.t = atof(top);
        }
        xmlFree(top);
        
        cxChar *left = cxXMLAttr("left");
        if(left != NULL){
            item.mask |= cxViewAutoResizeLeft;
            item.box.l = atof(left);
        }
        xmlFree(left);
        
        cxChar *right = cxXMLAttr("right");
        if(right != NULL){
            item.mask |= cxViewAutoResizeRight;
            item.box.r = atof(right);
        }
        xmlFree(right);
        
        cxChar *bottom = cxXMLAttr("bottom");
        if(bottom != NULL){
            item.mask |= cxViewAutoResizeBottom;
            item.box.b = atof(bottom);
        }
        xmlFree(bottom);
        
        cxChar *fill = cxXMLAttr("fill");
        if(cxConstCharsEqu(fill, "true")){
            item.box = cxBox4fv(0, 0, 0, 0);
            item.mask = cxViewAutoResizeFill;
        }
        xmlFree(fill);
        
        item.pos.x = cxXMLReadFloatAttr(reader, "x", 0);
        item.pos.y = cxXMLReadFloatAttr(reader, "y", 0);
        
        item.size.w = cxXMLReadFloatAttr(reader, "w", 0);
        item.size.h = cxXMLReadFloatAttr(reader, "h", 0);
        
        item.color = cxColor4fv(1, 1, 1, 1);
        cxXMLReadFloatsAttr(reader, "color", &item.color.r);
        
        item.texbox = cxBoxTex2fDefault();
        cxChar *skey = cxXMLAttr("key");
        if(skey != NULL){
            item.texbox = cxTextureBox(texture, skey);
            cxTypesSet(types, skey, cxNumberInt(index));
        }
        if(skey != NULL && cxSize2Zero(item.size)){
            item.size = cxTextureSize(texture, skey);
        }
        xmlFree(skey);
        index ++;
        cxTypesAppend(types, item);
    }
    return types;
}

cxTypes cxHashXMLReadLangString(xmlTextReaderPtr reader)
{
    cxTypes types = cxLangStringTypesCreate();
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(!ELEMENT_IS_TYPE(item)){
            continue;
        }
        cxChar *skey = cxXMLAttr("name");
        cxChar *svalue = cxXMLReadString(reader);
        if(skey != NULL && svalue != NULL){
            cxString string = cxStringAllocChars(svalue);
            cxTypesSet(types, skey, string);
            CX_RELEASE(string);
        }
        xmlFree(svalue);
        xmlFree(skey);
    }
    return types;
}

cxTypes cxHashXMLReadDB(xmlTextReaderPtr reader)
{
    cxChar *file = cxXMLAttr("file");
    cxChar *table = cxXMLAttr("table");
    cxChar *type = cxXMLAttr("type");
    cxAny db = NULL;
    if(file == NULL || table == NULL || type == NULL){
        CX_ERROR("cxDB must set file table type");
        return NULL;
    }
    if(strcasecmp(type, "btree") == 0){
        db = cxDBTreeCreate(cxStringConstChars(file), cxStringConstChars(table));
    }else if(strcasecmp(type, "hash") ==0){
        db = cxDBHashCreate(cxStringConstChars(file), cxStringConstChars(table));
    }else{
        CX_ERROR("type is btree or hash");
    }
    xmlFree(file);
    xmlFree(table);
    xmlFree(type);
    CX_RETURN(db == NULL, NULL);
    return cxDBTypesCreate(db);
}

cxTypes cxHashXMLReadString(xmlTextReaderPtr reader)
{
    cxTypes types = cxStringTypesCreate();
    cxString bytes = NULL;
    cxChar *src = cxXMLAttr("src");
    if(src != NULL){
        cxStream stream = cxAssetsStreamCreate(src);
        bytes = cxSreamBytes(stream);
    }else if(!xmlTextReaderIsEmptyElement(reader)) {
        bytes = cxStringConstChars(cxXMLReadString(reader));
    }else{
        bytes = NULL;
    }
    xmlFree(src);
    if(bytes == NULL){
        return NULL;
    }
    CX_RETAIN_SWAP(types->assist, bytes);
    return types;
}

/*
 <cxHash id="config">
    <cxString key="name">text</cxString>
    <cxInt key="int">123</cxInt>
    <cxFloat key="float">456</cxInt>
    <cxDouble key="double">789</cxInt>
    <cxVec2f key="vec2f">12,23</cxVec2f>
    <cxSize2f key="size2f">100,200</cxSize2f>
 </cxHash>
*/

cxTypes cxHashXMLReadHash(xmlTextReaderPtr reader)
{
    cxNumberValue vm={0};
    cxTypes types = cxHashTypesCreate();
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxChar *key = cxXMLAttr("key");
        if(key == NULL){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        cxChar *value = cxXMLReadString(reader);
        if(value == NULL){
            xmlFree(key);
            continue;
        }
        if(ELEMENT_IS_TYPE(cxString)){
            cxTypesSet(types, key, cxStringConstChars(value));
        }else if(ELEMENT_IS_TYPE(cxInt)){
            cxTypesSet(types, key, cxNumberInt(atoi(value)));
        }else if(ELEMENT_IS_TYPE(cxFloat)){
            cxTypesSet(types, key, cxNumberFloat(atof(value)));
        }else if (ELEMENT_IS_TYPE(cxDouble)){
            cxTypesSet(types, key, cxNumberDouble(atof(value)));
        }else if (ELEMENT_IS_TYPE(cxVec2f) && cxReadFloats(value, &vm.vec2f.x) == 2){
            cxTypesSet(types, key, cxNumberVec2f(vm.vec2f));
        }else if (ELEMENT_IS_TYPE(cxSize2f) && cxReadFloats(value, &vm.size2f.w) == 2){
            cxTypesSet(types, key, cxNumberSize2f(vm.size2f));
        }else if(ELEMENT_IS_TYPE(cxColor4f) && cxReadFloats(value, &vm.color4f.r) == 4){
            cxTypesSet(types, key, cxNumberColor4f(vm.color4f));
        }else if(ELEMENT_IS_TYPE(cxHash)){
            cxTypesSet(types, key, cxHashXMLReadHash(reader));
        }else{
            CX_ERROR("%s element key=%s value=%s", temp, key, value);
        }
        xmlFree(key);
        xmlFree(value);
    }
    return types;
}

CX_OBJECT_INIT(cxHashXML, cxObject)
{
    this->items = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxHashXML, cxObject)
{
    CX_RELEASE(this->items);
}
CX_OBJECT_TERM(cxHashXML, cxObject)

static void cxHashXMLReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxHashXML this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

cxBool cxHashXMLLoad(cxAny hash,cxConstChars xml)
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
    cxBool ret = false;
    xmlTextReaderPtr reader = xmlReaderForMemory(cxStringBody(script->bytes), cxStringLength(script->bytes), NULL, "UTF-8", 0);
    if(reader == NULL){
        CX_ERROR("create xml reader failed");
        return false;
    }
    xmlTextReaderSetErrorHandler(reader, cxHashXMLReaderError, hash);
    ret = cxHashXMLLoadWithReader(hash, reader);
    xmlFreeTextReader(reader);
    return ret;
}

cxBool cxHashXMLLoadWithReader(cxAny hash,xmlTextReaderPtr reader)
{
    cxBool ret = false;
    cxHashXML xml = hash;
    while(xmlTextReaderRead(reader)){
        if(xml->isError){
            break;
        }
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(ELEMENT_IS_TYPE(cxHashXML)){
            ret = true;
            break;
        }
    }
    if(!ret){
        return false;
    }
    cxAutoPoolPush();
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT){
            cxChar *sid = cxXMLAttr("id");
            if(sid == NULL){
                CX_ERROR("must set id");
                continue;
            }
            const xmlChar *temp = xmlTextReaderConstName(reader);
            cxAny object = NULL;
            if(ELEMENT_IS_TYPE(cxAtlasBoxPoint)){
                cxChar *stexture = cxXMLAttr("texture");
                object = cxHashXMLReadAtlasBoxPoint(stexture,reader);
                xmlFree(stexture);
            }else if(ELEMENT_IS_TYPE(cxLangString)){
                object = cxHashXMLReadLangString(reader);
            }else if(ELEMENT_IS_TYPE(cxDB)){
                object = cxHashXMLReadDB(reader);
            }else if(ELEMENT_IS_TYPE(cxString)){
                object = cxHashXMLReadString(reader);
            }else if(ELEMENT_IS_TYPE(cxHash)){
                object = cxHashXMLReadHash(reader);
            }
            if(object != NULL){
                cxHashSet(xml->items, cxHashStrKey(sid), object);
            }
            xmlFree(sid);
        }
    }
    cxAutoPoolPop();
    return ret;
}











