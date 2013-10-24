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
        cxChar *swidth = cxXMLAttr("width");
        cxChar *sheight = cxXMLAttr("height");
        cxChar *simagePath = cxXMLAttr("imagePath");
        if(swidth != NULL && sheight != NULL && simagePath != NULL){
            xml->innerTexture = cxTextureLoadFile((cxConstChars)simagePath);
            ret = (xml->innerTexture != NULL);
            CX_RETAIN(xml->innerTexture);
            xml->super.size.w = atof(swidth);
            xml->super.size.h = atof(sheight);
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
            cxChar *sn = cxXMLAttr("n");
            cxChar *sr = cxXMLAttr("r");
            e->isRotation = sr != NULL;
            
            cxChar *sx = cxXMLAttr("x");
            e->x = sx != NULL ? atof(sx) : 0;
            
            cxChar *sox = cxXMLAttr("oX");
            e->oX = sox != NULL ? atof(sox) : 0;
            
            cxChar *sy = cxXMLAttr("y");
            e->y = sy != NULL ? atof(sy) : 0;
            
            cxChar *soy = cxXMLAttr("oY");
            e->oY = soy != NULL ? atof(soy) : 0;
            
            cxChar *sw = cxXMLAttr("w");
            e->w = sw != NULL ? atof(sw) : 0;
            
            cxChar *sow = cxXMLAttr("oW");
            e->oW = sow != NULL ? atof(sow) : 0;
            
            cxChar *sh = cxXMLAttr("h");
            e->h = sh != NULL ? atof(sh) : 0;
            
            cxChar *soh = cxXMLAttr("oH");
            e->oH = soh != NULL ? atof(soh) : 0;
            
            if(sn != NULL && sx != NULL && sy != NULL && sw != NULL && sh != NULL){
                cxHashSet(xml->super.keys, cxHashStrKey(sn), e);
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



