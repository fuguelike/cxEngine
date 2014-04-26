//
//  cxBMPFont.c
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <textures/cxTextureFactory.h>
#include "cxJson.h"
#include "cxBMPFont.h"

CX_OBJECT_INIT(cxBMPElement, cxObject)
{
    
}
CX_OBJECT_FREE(cxBMPElement, cxObject)
{
    
}
CX_OBJECT_TERM(cxBMPElement, cxObject)

CX_OBJECT_INIT(cxBMPKerning, cxObject)
{
    
}
CX_OBJECT_FREE(cxBMPKerning, cxObject)
{
    
}
CX_OBJECT_TERM(cxBMPKerning, cxObject)


CX_OBJECT_INIT(cxBMPFont, cxObject)
{
    this->textures = CX_ALLOC(cxHash);
    this->chars = CX_ALLOC(cxHash);
    this->kernings = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxBMPFont, cxObject)
{
    CX_RELEASE(this->face);
    CX_RELEASE(this->chasrset);
    CX_RELEASE(this->textures);
    CX_RELEASE(this->chars);
    CX_RELEASE(this->kernings);
}
CX_OBJECT_TERM(cxBMPFont, cxObject)

cxTexture cxBMPFontTexture(cxBMPFont this,cxUChar page)
{
    return cxHashGet(this->textures, cxHashIntKey(page));
}

cxFloat cxBMPKerningAmount(cxBMPFont this, cxUInt16 prev,cxUInt16 curr)
{
    if(prev == 0){
        return 0;
    }
    cxInt key = cxBMPKerningKey(prev, curr);
    cxBMPKerning bk = cxHashGet(this->kernings, cxHashIntKey(key));
    if(bk == NULL){
        return 0;
    }
    return bk->amount;
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
    cxJson json = cxJsonCreate(data);
    if(json == NULL){
        CX_ERROR("load bmpfont error file %s not json",file);
        return false;
    }
    cxJson info = cxJsonObject(json, "info");
    this->face = cxJsonString(info, "face");
    CX_RETAIN(this->face);
    this->size = cxJsonInt(info, "size", 0);
    this->bold = cxJsonInt(info, "bold", 0);
    this->italic = cxJsonInt(info, "italic", 0);
    this->chasrset = cxJsonString(info, "chasrset");
    CX_RETAIN(this->chasrset);
    this->unicode = cxJsonInt(info, "unicode", 0);
    this->stretchH = cxJsonInt(info, "stretchH", 0);
    this->smooth = cxJsonInt(info, "smooth", 0);
    this->aa = cxJsonInt(info, "aa", 0);
    
    cxJson common = cxJsonObject(json, "common");
    this->lineHeight = cxJsonInt(common, "lineHeight", 0);
    this->base = cxJsonInt(common, "base", 0);
    this->scale.w = cxJsonInt(common, "scaleW", 0);
    this->scale.h = cxJsonInt(common, "scaleH", 0);
    this->pages = cxJsonInt(common, "pages", 0);
    this->packed = cxJsonInt(common, "packed", 0);

    cxJson pages = cxJsonArray(json, "pages");
    CX_JSON_ARRAY_EACH_BEG(pages, item)
    {
        cxInt id = cxJsonInt(item, "id", 0);
        cxConstChars file = cxJsonConstChars(item, "file");
        CX_ASSERT(file != NULL, "file null string");
        cxTexture texture = cxTextureFactoryLoadFile(file);
        CX_ASSERT(texture != NULL, "create bmp texture failed %s",file);
        cxHashSet(this->textures, cxHashIntKey(id), texture);
    }
    CX_JSON_ARRAY_EACH_END(pages, item)
    
    cxJson chars = cxJsonArray(json, "chars");
    CX_JSON_ARRAY_EACH_BEG(chars, item)
    {
        cxBMPElement pchar = CX_ALLOC(cxBMPElement);
        pchar->key = cxJsonInt(item, "id", 0);
        pchar->box.x = cxJsonInt(item, "x", 0);
        pchar->box.y = cxJsonInt(item, "y", 0);
        pchar->box.w = cxJsonInt(item, "width", 0);
        pchar->box.h = cxJsonInt(item, "height", 0);
        pchar->xoffset = cxJsonInt(item, "xoffset", 0);
        pchar->yoffset = cxJsonInt(item, "yoffset", 0);
        pchar->xadvance = cxJsonInt(item, "xadvance", 0);
        pchar->page = cxJsonInt(item, "page", 0);
        cxHashSet(this->chars, cxHashIntKey(pchar->key), pchar);
        CX_RELEASE(pchar);
    }
    CX_JSON_ARRAY_EACH_END(chars, item)
    
    cxJson kernings = cxJsonArray(json, "kernings");
    CX_JSON_ARRAY_EACH_BEG(kernings, item)
    {
        cxBMPKerning kerning = CX_ALLOC(cxBMPKerning);
        kerning->first = cxJsonInt(item, "first", 0);
        kerning->second = cxJsonInt(item, "second", 0);
        kerning->amount = cxJsonInt(item, "amount", 0);
        cxInt key = cxBMPKerningKey(kerning->first, kerning->second);
        cxHashSet(this->kernings, cxHashIntKey(key), kerning);
        CX_RELEASE(kerning);
    }
    CX_JSON_ARRAY_EACH_END(kernings, item)
    return true;
}

















