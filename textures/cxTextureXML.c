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
    cxString data = cxStreamAllBytes(stream);
    if(data == NULL){
        CX_ERROR("read data failed from stream");
        return ret;
    }
    xmlTextReaderPtr reader = cxXMLReaderForString(data, NULL, NULL);
    cxReaderAttrInfo *info = cxReaderAttrInfoMake(reader, NULL, this);
    if(reader == NULL){
        CX_ERROR("xml reader from memory failed");
        return ret;
    }
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        if(!ELEMENT_IS_TYPE(TextureAtlas)){
            continue;
        }
        cxConstChars simagePath = cxXMLAttr(reader,"imagePath");
        CX_ASSERT(simagePath != NULL, "xml imagePath element miss");
    
        xml->innerTexture = cxTextureFactoryLoadFile(simagePath);
        ret = (xml->innerTexture != NULL);
        CX_ASSERT(ret, "xml innert texture error");
        CX_RETAIN(xml->innerTexture);
        
        //for jpg pkm atlas texture
        xml->super.isAtlas = cxXMLReadBoolAttr(info,"atlas", false);
        xml->super.size = xml->innerTexture->size;

        //load texcoords
        while(xmlTextReaderRead(reader)){
            if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
                continue;
            }
            cxConstChars temp = cxXMLReadElementName(reader);
            if(!ELEMENT_IS_TYPE(sprite)){
                continue;
            }
            cxTexCoord e = CX_ALLOC(cxTexCoord);
            cxConstChars sn = cxXMLAttr(reader,"n");
            cxConstChars sr = cxXMLAttr(reader,"r");
            e->isRotation = sr != NULL;
            e->x = cxXMLReadFloatAttr(info, "x", 0);
            e->y = cxXMLReadFloatAttr(info, "y", 0);
            e->w = cxXMLReadFloatAttr(info, "w", 0);
            e->h = cxXMLReadFloatAttr(info, "h", 0);
            cxHashSet(xml->super.keys, cxHashStrKey(sn), e);
            CX_RELEASE(e);
        }
    }
    return ret;
}

static void cxTextureXMLBind(cxAny this)
{
    cxTextureXML xml = this;
    CX_ASSERT(xml->innerTexture != NULL, "xml inner texture error");
    cxTextureBind(xml->innerTexture);
}

CX_OBJECT_INIT(cxTextureXML, cxTexture)
{
    CX_METHOD_OVERRIDE(this->super.Bind, cxTextureXMLBind);
    CX_METHOD_OVERRIDE(this->super.Load, cxTextureXMLLoad);
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



