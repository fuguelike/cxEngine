//
//  cxBMPFont.c
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <textures/cxTextureFactory.h>
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
    cxString data = cxAssetsData(file);
    if(data == NULL){
        CX_ERROR("load bmpfont error file %s not exists",file);
        return false;
    }
//    xmlTextReaderPtr reader = cxXMLReaderForString(data, cxBMPFontReaderError, this);
//    cxReaderAttrInfo *info = cxReaderAttrInfoMake(reader, NULL, this);
//    while(xmlTextReaderRead(reader) && !this->isError){
//        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
//            continue;
//        }
//        cxConstChars temp = cxXMLReadElementName(reader);
//        if(ELEMENT_IS_TYPE(info)){
//            this->face = cxXMLReadStringAttr(info, "face");
//            CX_RETAIN(this->face);
//            this->size = cxXMLReadIntAttr(info, "size", 0);
//            this->bold = cxXMLReadIntAttr(info, "bold", 0);
//            this->italic = cxXMLReadIntAttr(info, "italic", 0);
//            this->chasrset = cxXMLReadStringAttr(info, "chasrset");
//            CX_RETAIN(this->chasrset);
//            this->unicode = cxXMLReadIntAttr(info, "unicode", 0);
//            this->stretchH = cxXMLReadIntAttr(info, "stretchH", 0);
//            this->smooth = cxXMLReadIntAttr(info, "smooth", 0);
//            this->aa = cxXMLReadIntAttr(info, "aa", 0);
//            this->padding = cxXMLReadBox4iAttr(info, "padding", this->padding);
//            this->spacing = cxXMLReadVec2iAttr(info, "spacing", this->spacing);
//        }else if(ELEMENT_IS_TYPE(common)){
//            this->lineHeight = cxXMLReadIntAttr(info, "lineHeight", 0);
//            this->base = cxXMLReadIntAttr(info, "base", 0);
//            this->scale.w = cxXMLReadIntAttr(info, "scaleW", 0);
//            this->scale.h = cxXMLReadIntAttr(info, "scaleH", 0);
//            this->pages = cxXMLReadIntAttr(info, "pages", 0);
//            this->packed = cxXMLReadIntAttr(info, "packed", 0);
//        }else if(ELEMENT_IS_TYPE(page)){
//            cxInt id = cxXMLReadIntAttr(info, "id", 0);
//            cxConstChars file = cxXMLAttr(reader, "file");
//            cxTexture texture = cxTextureFactoryLoadFile(file);
//            CX_ASSERT(texture != NULL, "create bmp texture failed");
//            cxHashSet(this->textures, cxHashIntKey(id), texture);
//        }else if(ELEMENT_IS_TYPE(char)){
//            cxBMPElement pchar = CX_ALLOC(cxBMPElement);
//            cxUInt id = cxXMLReadUIntAttr(info, "id", 0);
//            pchar->box.x = cxXMLReadIntAttr(info, "x", 0);
//            pchar->box.y = cxXMLReadIntAttr(info, "y", 0);
//            pchar->box.w = cxXMLReadIntAttr(info, "width", 0);
//            pchar->box.h = cxXMLReadIntAttr(info, "height", 0);
//            pchar->xoffset = cxXMLReadIntAttr(info, "xoffset", 0);
//            pchar->yoffset = cxXMLReadIntAttr(info, "yoffset", 0);
//            pchar->xadvance = cxXMLReadIntAttr(info, "xadvance", 0);
//            pchar->page = cxXMLReadIntAttr(info, "page", 0);
//            cxHashSet(this->chars, cxHashIntKey(id), pchar);
//            CX_RELEASE(pchar);
//        }
//    }
    return true;
}

















