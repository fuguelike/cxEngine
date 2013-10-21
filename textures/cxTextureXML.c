//
//  cxTextureXML.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <libxml/xmlreader.h>
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
        xmlChar *swidth = xmlTextReaderGetAttribute(reader, BAD_CAST"width");
        xmlChar *sheight = xmlTextReaderGetAttribute(reader, BAD_CAST"height");
        xmlChar *simagePath = xmlTextReaderGetAttribute(reader, BAD_CAST"imagePath");
        if(swidth != NULL && sheight != NULL && simagePath != NULL){
            xml->innerTexture = cxTextureLoadFile((cxConstChars)simagePath);
            ret = (xml->innerTexture != NULL);
            CX_RETAIN(xml->innerTexture);
            xml->super.size.w = atof((cxConstChars)swidth);
            xml->super.size.h = atof((cxConstChars)sheight);
        }
        xmlFree(swidth);
        xmlFree(sheight);
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
            xmlChar *sn = xmlTextReaderGetAttribute(reader, BAD_CAST"n");
            xmlChar *sr = xmlTextReaderGetAttribute(reader, BAD_CAST"r");
            e->isRotation = sr != NULL;
            
            xmlChar *sx = xmlTextReaderGetAttribute(reader, BAD_CAST"x");
            e->x = sx != NULL ? atof((cxConstChars)sx) : 0;
            
            xmlChar *sox = xmlTextReaderGetAttribute(reader, BAD_CAST"oX");
            e->oX = sox != NULL ? atof((cxConstChars)sox) : 0;
            
            xmlChar *sy = xmlTextReaderGetAttribute(reader, BAD_CAST"y");
            e->y = sy != NULL ? atof((cxConstChars)sy) : 0;
            
            xmlChar *soy = xmlTextReaderGetAttribute(reader, BAD_CAST"oY");
            e->oY = soy != NULL ? atof((cxConstChars)soy) : 0;
            
            xmlChar *sw = xmlTextReaderGetAttribute(reader, BAD_CAST"w");
            e->w = sw != NULL ? atof((cxConstChars)sw) : 0;
            
            xmlChar *sow = xmlTextReaderGetAttribute(reader, BAD_CAST"oW");
            e->oW = sow != NULL ? atof((cxConstChars)sow) : 0;
            
            xmlChar *sh = xmlTextReaderGetAttribute(reader, BAD_CAST"h");
            e->h = sh != NULL ? atof((cxConstChars)sh) : 0;
            
            xmlChar *soh = xmlTextReaderGetAttribute(reader, BAD_CAST"oH");
            e->oH = soh != NULL ? atof((cxConstChars)soh) : 0;
            
            if(sn != NULL && sx != NULL && sy != NULL && sw != NULL && sh != NULL){
                cxHashSet(xml->super.keys, cxHashStrKey((cxConstChars)sn), e);
            }
            CX_RELEASE(e);
            xmlFree(sn);
            xmlFree(sr);
            xmlFree(sx);
            xmlFree(sy);
            xmlFree(sw);
            xmlFree(sh);
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



