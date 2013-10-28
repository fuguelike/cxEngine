//
//  cxTextureXML.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxXMLScript.h>
#include <streams/cxAssetsStream.h>
#include "cxTextureXML.h"
#include "cxTextureFactory.h"

static cxBool cxTextureXMLLoad(cxAny this,cxStream stream)
{
    cxBool ret = false;
    cxTextureXML xml = this;
    CX_ASSERT(stream != NULL, "stream not set");
    cxString data = stream->interface->AllBytes(stream);
    if(data == NULL){
        CX_ERROR("read data failed from stream");
        return ret;
    }
    xmlTextReaderPtr reader = xmlReaderForMemory(cxStringBody(data), cxStringLength(data), NULL, "UTF-8", 0);
    if(reader == NULL){
        CX_ERROR("xml reader fro memory failed");
        return ret;
    }
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(xmlStrcmp(temp, BAD_CAST"TextureAtlas") != 0){
            continue;
        }
        cxChar *simagePath = cxXMLAttr("imagePath");
        if(simagePath != NULL){
            xml->innerTexture = cxTextureLoadFile((cxConstChars)simagePath);
            ret = (xml->innerTexture != NULL);
            CX_RETAIN(xml->innerTexture);
            xml->super.size.w = cxXMLReadFloatAttr(reader, "width", 0);
            xml->super.size.h = cxXMLReadFloatAttr(reader, "height", 0);
        }
        xmlFree(simagePath);
        if(xml->innerTexture == NULL){
            CX_ERROR("get xml inner texture failed");
            break;
        }
        //load texcoords
        while(xmlTextReaderRead(reader)){
            if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
                continue;
            }
            const xmlChar *temp = xmlTextReaderConstName(reader);
            if(xmlStrcmp(temp, BAD_CAST"sprite") != 0){
                continue;
            }
            cxTexCoord e = CX_ALLOC(cxTexCoord);
            cxChar *sn = cxXMLAttr("n");
            cxChar *sr = cxXMLAttr("r");
            e->isRotation = sr != NULL;
            e->x = cxXMLReadFloatAttr(reader, "x", 0);
            e->oX = cxXMLReadFloatAttr(reader, "oX", 0);
            e->y = cxXMLReadFloatAttr(reader, "y", 0);
            e->oY = cxXMLReadFloatAttr(reader, "oY", 0);
            e->w = cxXMLReadFloatAttr(reader, "w", 0);
            e->oW = cxXMLReadFloatAttr(reader, "oW", 0);
            e->h = cxXMLReadFloatAttr(reader, "h", 0);
            e->oH = cxXMLReadFloatAttr(reader, "oH", 0);
            cxHashSet(xml->super.keys, cxHashStrKey(sn), e);
            CX_RELEASE(e);
            xmlFree(sn);
            xmlFree(sr);
        }
    }
    xmlFreeTextReader(reader);
    CX_ASSERT(xml->super.size.w == xml->innerTexture->size.w, "xml width != texture width");
    CX_ASSERT(xml->super.size.h == xml->innerTexture->size.h, "xml height != texture height");
    return ret;
}

static void cxTextureXMLBind(cxAny this)
{
    cxTextureXML xml = this;
    if(xml->innerTexture != NULL){
        cxTextureBind(xml->innerTexture);
    }
}

static const cxTextureInterface xmlInterface = {
    .Load = cxTextureXMLLoad,
    .Bind = cxTextureXMLBind,
};

CX_OBJECT_INIT(cxTextureXML, cxTexture)
{
    this->super.interface = &xmlInterface;
}
CX_OBJECT_FREE(cxTextureXML, cxTexture)
{
    CX_RELEASE(this->innerTexture);
}
CX_OBJECT_TERM(cxTextureXML, cxTexture)

cxTexture cxTextureXMLLoadStream(cxStream stream)
{
    cxTextureXML xml = CX_CREATE(cxTextureXML);
    cxTextureLoad((cxTexture)xml,stream);
    return (cxTexture)xml;
}



