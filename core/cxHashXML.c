//
//  cxHashXML.c
//  cxEngine
//
//  Created by xuhua on 10/19/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <textures/cxTextureFactory.h>
#include <views/cxAtlas.h>
#include "cxEngine.h"
#include "cxAutoPool.h"
#include "cxStack.h"
#include "cxHashXML.h"

cxTypes cxHashXMLReadBoxPoint(cxConstChars texfile,xmlTextReaderPtr reader)
{
    cxTypes types = cxBoxPointTypesCreate();
    cxTexture texture = cxTextureLoadFile(texfile);
    if(texture == NULL){
        CX_ERROR("boxPoint must set textfile at cxBoxPoint node");
        return NULL;
    }
    types->assist = cxStringAllocChars(texfile);
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(!ELEMENT_IS_TYPE(item)){
            continue;
        }
        cxVec2f pos;
        pos.x = cxXMLReadFloatAttr(reader, "x", 0);
        pos.y = cxXMLReadFloatAttr(reader, "y", 0);
        
        cxSize2f size;
        size.w = cxXMLReadFloatAttr(reader, "w", 0);
        size.h = cxXMLReadFloatAttr(reader, "h", 0);
        cxColor4f color = cxColor4fv(1, 1, 1, 1);
        cxXMLReadFloatsAttr(reader, "color", &color.r);
        
        xmlChar *skey = cxXMLAttr("key");
        if(skey != NULL && (size.w ==0 || size.h == 0)){
            size = cxTextureSize(texture, (cxConstChars)skey);
        }
        cxBoxTex2f texBox = cxTextureBox(texture, (cxConstChars)skey);
        xmlFree(skey);
        
        cxBoxPoint box = cxAtlasCreateBoxPoint(pos, size, texBox, color);
        cxTypesAppend(types, box);
    }
    return types;
}

cxTypes cxHashXMLReadString(xmlTextReaderPtr reader)
{
    cxTypes types = CX_CREATE(cxTypes);
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(!ELEMENT_IS_TYPE(item)){
            continue;
        }
        xmlChar *skey = cxXMLAttr("key");
        xmlChar *svalue = xmlTextReaderReadString(reader);
        cxTypesAppendKeyValue(types, (cxConstChars)skey, cxStringConstChars((cxConstChars)svalue));
        xmlFree(svalue);
        xmlFree(skey);
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
        int type = xmlTextReaderNodeType(reader);
        if(type == XML_READER_TYPE_ELEMENT){
            xmlChar *sid = cxXMLAttr("id");
            if(sid == NULL){
                CX_ERROR("must set id");
                continue;
            }
            const xmlChar *temp = xmlTextReaderConstName(reader);
            cxAny object = NULL;
            if(ELEMENT_IS_TYPE(cxBoxPoint)){
                xmlChar *stexture = cxXMLAttr("texture");
                object = cxHashXMLReadBoxPoint((cxConstChars)stexture,reader);
                xmlFree(stexture);
            }else if(ELEMENT_IS_TYPE(cxString)){
                object = cxHashXMLReadString(reader);
            }
            if(object != NULL){
                cxHashSet(xml->items, cxHashStrKey((cxConstChars)sid), object);
            }
            xmlFree(sid);
        }
    }
    cxAutoPoolPop();
    return ret;
}











