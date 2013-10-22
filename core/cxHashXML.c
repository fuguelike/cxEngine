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

cxTypes cxHashXMLReadAtlasBoxPoint(cxConstChars texfile,xmlTextReaderPtr reader)
{
    cxTypes types = cxAtlasBoxPointTypesCreate();
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
        if(fill != NULL && strcasecmp(fill, "true") == 0){
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
        }
        if(skey != NULL && cxSize2Zero(item.size)){
            item.size = cxTextureSize(texture, skey);
        }
        xmlFree(skey);
        
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
        cxChar *skey = cxXMLAttr("key");
        cxChar *svalue = cxXMLReadString(reader);
        cxTypesSet(types, skey, svalue);
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











