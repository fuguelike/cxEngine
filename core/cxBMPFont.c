//
//  cxBMPFont.c
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <textures/cxTextureFactory.h>
#include "cxXMLScript.h"
#include "cxBMPFont.h"

CX_OBJECT_INIT(cxBMPElement, cxObject)
{
    
}
CX_OBJECT_FREE(cxBMPElement, cxObject)
{
    
}
CX_OBJECT_TERM(cxBMPElement, cxObject)


CX_OBJECT_INIT(cxBMPFont, cxObject)
{
    this->textures = CX_ALLOC(cxHash);
    this->chars = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxBMPFont, cxObject)
{
    CX_RELEASE(this->face);
    CX_RELEASE(this->chasrset);
    CX_RELEASE(this->textures);
    CX_RELEASE(this->chars);
}
CX_OBJECT_TERM(cxBMPFont, cxObject)

static void cxBMPFontReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxBMPFont this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

cxBMPFont cxBMPFontCreate(cxConstChars file)
{
    cxBMPFont font = CX_CREATE(cxBMPFont);
    if(cxBMPFontLoad(font, file)){
        return font;
    }
    return NULL;
}

cxBool cxBMPFontLoad(cxBMPFont this,cxConstChars file)
{
    cxString data = cxAssertsData(file);
    if(data == NULL){
        CX_ERROR("load bmpfont error file %s not exists",file);
        return false;
    }
    xmlTextReaderPtr reader = cxXMLReaderForString(data);
    xmlTextReaderSetErrorHandler(reader, cxBMPFontReaderError, this);
    
    while(xmlTextReaderRead(reader) && !this->isError){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(ELEMENT_IS_TYPE(info)){
            this->face = cxXMLReadAllocAttr(reader, "face");
            this->size = cxXMLReadIntAttr(reader, "size", 0);
            this->bold = cxXMLReadIntAttr(reader, "bold", 0);
            this->italic = cxXMLReadIntAttr(reader, "italic", 0);
            this->chasrset = cxXMLReadAllocAttr(reader, "chasrset");
            this->unicode = cxXMLReadIntAttr(reader, "unicode", 0);
            this->stretchH = cxXMLReadIntAttr(reader, "stretchH", 0);
            this->smooth = cxXMLReadIntAttr(reader, "smooth", 0);
            this->aa = cxXMLReadIntAttr(reader, "aa", 0);
            cxXMLReadIntsAttr(reader, "padding", &this->padding.l);
            cxXMLReadIntsAttr(reader, "spacing", &this->spacing.x);
        }else if(ELEMENT_IS_TYPE(common)){
            this->lineHeight = cxXMLReadIntAttr(reader, "lineHeight", 0);
            this->base = cxXMLReadIntAttr(reader, "base", 0);
            this->scale.w = cxXMLReadIntAttr(reader, "scaleW", 0);
            this->scale.h = cxXMLReadIntAttr(reader, "scaleH", 0);
            this->pages = cxXMLReadIntAttr(reader, "pages", 0);
            this->packed = cxXMLReadIntAttr(reader, "packed", 0);
        }else if(ELEMENT_IS_TYPE(page)){
            cxInt id = cxXMLReadIntAttr(reader, "id", 0);
            cxChar *file = cxXMLAttr("file");
            cxTexture texture = cxTextureCreate(file);
            CX_ASSERT(texture != NULL, "create bmp texture failed");
            cxHashSet(this->textures, cxHashIntKey(id), texture);
            xmlFree(file);
        }else if(ELEMENT_IS_TYPE(char)){
            cxBMPElement pchar = CX_ALLOC(cxBMPElement);
            pchar->id = cxXMLReadUIntAttr(reader, "id", 0);
            pchar->box.x = cxXMLReadIntAttr(reader, "x", 0);
            pchar->box.y = cxXMLReadIntAttr(reader, "y", 0);
            pchar->box.w = cxXMLReadIntAttr(reader, "width", 0);
            pchar->box.h = cxXMLReadIntAttr(reader, "height", 0);
            pchar->xoffset = cxXMLReadIntAttr(reader, "xoffset", 0);
            pchar->yoffset = cxXMLReadIntAttr(reader, "yoffset", 0);
            pchar->xadvance = cxXMLReadIntAttr(reader, "xadvance", 0);
            pchar->page = cxXMLReadIntAttr(reader, "page", 0);
            cxHashSet(this->chars, cxHashIntKey(pchar->id), pchar);
            CX_RELEASE(pchar);
        }
    }
    xmlTextReaderClose(reader);
    return true;
}

















