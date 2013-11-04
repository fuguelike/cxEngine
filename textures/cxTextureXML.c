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
    xmlTextReaderPtr reader = cxXMLReaderForString(data);
    if(reader == NULL){
        CX_ERROR("xml reader from memory failed");
        return ret;
    }
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(!ELEMENT_IS_TYPE(TextureAtlas)){
            continue;
        }
        cxChar *simagePath = cxXMLAttr("imagePath");
        CX_ASSERT(simagePath != NULL, "xml imagePath element miss");
        
        xml->innerTexture = cxTextureFactoryLoadFile((cxConstChars)simagePath);
        ret = (xml->innerTexture != NULL);
        CX_RETAIN(xml->innerTexture);
        
        xml->super.size.w = cxXMLReadFloatAttr(reader, "width", 0);
        xml->super.size.h = cxXMLReadFloatAttr(reader, "height", 0);
        
        CX_ASSERT(ret && cxSize2fEqu(xml->innerTexture->size, xml->super.size), "xml texture error");
        
        xmlFree(simagePath);
        if(!ret){
            CX_ERROR("get xml inner texture failed");
            break;
        }
        //load texcoords
        while(xmlTextReaderRead(reader)){
            if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
                continue;
            }
            const xmlChar *temp = xmlTextReaderConstName(reader);
            if(!ELEMENT_IS_TYPE(sprite)){
                continue;
            }
            cxTexCoord e = CX_ALLOC(cxTexCoord);
            cxChar *sn = cxXMLAttr("n");
            cxChar *sr = cxXMLAttr("r");
            e->isRotation = sr != NULL;
            e->x = cxXMLReadFloatAttr(reader, "x", 0);
            e->y = cxXMLReadFloatAttr(reader, "y", 0);
            e->w = cxXMLReadFloatAttr(reader, "w", 0);
            e->h = cxXMLReadFloatAttr(reader, "h", 0);
            cxHashSet(xml->super.keys, cxHashStrKey(sn), e);
            CX_RELEASE(e);
            xmlFree(sn);
            xmlFree(sr);
        }
    }
    xmlFreeTextReader(reader);
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
    cxTexture xml = CX_CREATE(cxTextureXML);
    cxTextureLoad(xml,stream);
    return xml;
}



