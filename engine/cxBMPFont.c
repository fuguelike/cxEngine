//
//  cxBMPFont.c
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <textures/cxTextureCache.h>
#include "cxBMPFont.h"

CX_TYPE(cxBMPElement, cxObject)
{}
CX_INIT(cxBMPElement, cxObject)
{}
CX_FREE(cxBMPElement, cxObject)
{}
CX_TERM(cxBMPElement, cxObject)


CX_TYPE(cxBMPKerning, cxObject)
{}
CX_INIT(cxBMPKerning, cxObject)
{}
CX_FREE(cxBMPKerning, cxObject)
{}
CX_TERM(cxBMPKerning, cxObject)


CX_TYPE(cxBMPFont, cxObject)
{}
CX_INIT(cxBMPFont, cxObject)
{
    this->textures = CX_ALLOC(cxHash);
    this->chars = CX_ALLOC(cxHash);
    this->kernings = CX_ALLOC(cxHash);
}
CX_FREE(cxBMPFont, cxObject)
{
    CX_RELEASE(this->face);
    CX_RELEASE(this->chasrset);
    CX_RELEASE(this->textures);
    CX_RELEASE(this->chars);
    CX_RELEASE(this->kernings);
}
CX_TERM(cxBMPFont, cxObject)

cxTexture cxBMPFontTexture(cxBMPFont this,cxUChar page)
{
    return cxHashGet(this->textures, cxHashIntKey(page));
}

cxFloat cxBMPKerningAmount(cxBMPFont this, cxUInt16 prev,cxUInt16 curr)
{
    CX_RETURN(prev == 0, 0);
    cxHashKey key = cxBMPKerningKey(prev, curr);
    cxBMPKerning bk = cxHashGet(this->kernings, key);
    CX_RETURN(bk == NULL, 0);
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
    cxJson json = cxEngineJsonReader(file);
    if(json == NULL){
        CX_ERROR("load bmpfont error file %s not json",file);
        return false;
    }
    //base attr
    cxJson info = cxJsonObject(json, "info");
    CX_RETAIN_SET(this->face, cxJsonStr(info, "face"));
    this->size          = cxJsonInt(info, "size", 0);
    this->bold          = cxJsonInt(info, "bold", 0);
    this->italic        = cxJsonInt(info, "italic", 0);
    CX_RETAIN_SET(this->chasrset, cxJsonStr(info, "chasrset"));
    this->unicode       = cxJsonInt(info, "unicode", 0);
    this->stretchH      = cxJsonInt(info, "stretchH", 0);
    this->smooth        = cxJsonInt(info, "smooth", 0);
    this->aa            = cxJsonInt(info, "aa", 0);
    //common attr
    cxJson common       = cxJsonObject(json, "common");
    this->lineHeight    = cxJsonInt(common, "lineHeight", 0);
    this->base          = cxJsonInt(common, "base", 0);
    this->scale.w       = cxJsonInt(common, "scaleW", 0);
    this->scale.h       = cxJsonInt(common, "scaleH", 0);
    this->pages         = cxJsonInt(common, "pages", 0);
    this->packed        = cxJsonInt(common, "packed", 0);
    //page attr
    cxJson pages = cxJsonArray(json, "pages");
    CX_JSON_ARRAY_EACH_BEG(pages, item)
    cxInt id = cxJsonInt(item, "id", 0);
    cxConstChars file = cxJsonConstChars(item, "file");
    CX_ASSERT(file != NULL, "file null string");
    cxTexture texture = cxTextureCacheLoadFile(file);
    CX_ASSERT(texture != NULL, "create bmp texture failed %s",file);
    cxHashSet(this->textures, cxHashIntKey(id), texture);
    CX_JSON_ARRAY_EACH_END(pages, item)
    //char attr
    cxJson chars = cxJsonArray(json, "chars");
    CX_JSON_ARRAY_EACH_BEG(chars, item)
    cxBMPElement pchar = CX_ALLOC(cxBMPElement);
    pchar->key      = cxJsonInt(item, "id", 0);
    pchar->box.x    = cxJsonInt(item, "x", 0);
    pchar->box.y    = cxJsonInt(item, "y", 0);
    pchar->box.w    = cxJsonInt(item, "width", 0);
    pchar->box.h    = cxJsonInt(item, "height", 0);
    pchar->xoffset  = cxJsonInt(item, "xoffset", 0);
    pchar->yoffset  = cxJsonInt(item, "yoffset", 0);
    pchar->xadvance = cxJsonInt(item, "xadvance", 0);
    pchar->page     = cxJsonInt(item, "page", 0);
    cxHashSet(this->chars, cxHashIntKey(pchar->key), pchar);
    CX_RELEASE(pchar);
    CX_JSON_ARRAY_EACH_END(chars, item)
    //kerning attr
    cxJson kernings = cxJsonArray(json, "kernings");
    CX_JSON_ARRAY_EACH_BEG(kernings, item)
    cxBMPKerning kerning = CX_ALLOC(cxBMPKerning);
    kerning->first = cxJsonInt(item, "first", 0);
    kerning->second = cxJsonInt(item, "second", 0);
    kerning->amount = cxJsonInt(item, "amount", 0);
    cxHashKey key = cxBMPKerningKey(kerning->first, kerning->second);
    cxHashSet(this->kernings, key, kerning);
    CX_RELEASE(kerning);
    CX_JSON_ARRAY_EACH_END(kernings, item)
    return true;
}

















