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

cxTexture cxBMPFontTexture(cxBMPFont this,cxUChar page)
{
    return cxHashGet(this->textures, cxHashIntKey(page));
}

cxBMPElement cxBMPFontChar(cxBMPFont this,cxUInt id)
{
    return cxHashGet(this->chars, cxHashIntKey(id));
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
    xmlTextReaderPtr reader = cxXMLReaderForString(data, cxBMPFontReaderError, this);
    while(xmlTextReaderRead(reader) && !this->isError){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        if(ELEMENT_IS_TYPE(info)){
            this->face = cxXMLReadStringAttr(reader, NULL, "face");
            CX_RETAIN(this->face);
            this->size = cxXMLReadIntAttr(reader, NULL, "size", 0);
            this->bold = cxXMLReadIntAttr(reader, NULL, "bold", 0);
            this->italic = cxXMLReadIntAttr(reader, NULL, "italic", 0);
            this->chasrset = cxXMLReadStringAttr(reader, NULL, "chasrset");
            CX_RETAIN(this->chasrset);
            this->unicode = cxXMLReadIntAttr(reader, NULL, "unicode", 0);
            this->stretchH = cxXMLReadIntAttr(reader, NULL, "stretchH", 0);
            this->smooth = cxXMLReadIntAttr(reader, NULL, "smooth", 0);
            this->aa = cxXMLReadIntAttr(reader, NULL, "aa", 0);
            this->padding = cxXMLReadBox4iAttr(reader, NULL, "padding", this->padding);
            this->spacing = cxXMLReadVec2iAttr(reader, NULL, "spacing", this->spacing);
        }else if(ELEMENT_IS_TYPE(common)){
            this->lineHeight = cxXMLReadIntAttr(reader, NULL, "lineHeight", 0);
            this->base = cxXMLReadIntAttr(reader, NULL, "base", 0);
            this->scale.w = cxXMLReadIntAttr(reader, NULL, "scaleW", 0);
            this->scale.h = cxXMLReadIntAttr(reader, NULL, "scaleH", 0);
            this->pages = cxXMLReadIntAttr(reader, NULL, "pages", 0);
            this->packed = cxXMLReadIntAttr(reader, NULL, "packed", 0);
        }else if(ELEMENT_IS_TYPE(page)){
            cxInt id = cxXMLReadIntAttr(reader, NULL, "id", 0);
            cxConstChars file = cxXMLAttr("file");
            cxTexture texture = cxTextureCreate(file);
            CX_ASSERT(texture != NULL, "create bmp texture failed");
            cxHashSet(this->textures, cxHashIntKey(id), texture);
        }else if(ELEMENT_IS_TYPE(char)){
            cxBMPElement pchar = CX_ALLOC(cxBMPElement);
            cxUInt id = cxXMLReadUIntAttr(reader, NULL, "id", 0);
            pchar->box.x = cxXMLReadIntAttr(reader, NULL, "x", 0);
            pchar->box.y = cxXMLReadIntAttr(reader, NULL, "y", 0);
            pchar->box.w = cxXMLReadIntAttr(reader, NULL, "width", 0);
            pchar->box.h = cxXMLReadIntAttr(reader, NULL, "height", 0);
            pchar->xoffset = cxXMLReadIntAttr(reader, NULL, "xoffset", 0);
            pchar->yoffset = cxXMLReadIntAttr(reader, NULL, "yoffset", 0);
            pchar->xadvance = cxXMLReadIntAttr(reader, NULL, "xadvance", 0);
            pchar->page = cxXMLReadIntAttr(reader, NULL, "page", 0);
            cxHashSet(this->chars, cxHashIntKey(id), pchar);
            CX_RELEASE(pchar);
        }
    }
    return true;
}

















