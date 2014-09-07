//
//  cxLabelBMP.c
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <engine/cxIconv.h>
#include "cxLabelBMP.h"

static void cxLabelBMPUpdateText(cxLabelBMP this)
{
    CX_RETURN(!cxStringLength(this->txt));
    //support 1 page
    cxTexture texture = cxBMPFontTexture(this->font, 0);
    CX_ASSERT(texture != NULL, "texture miss");
    cxSpriteSetTexture(this, texture);
    //convert to unicode
    cxString unicode = cxIconvConvertUTF8ToUTF16LE(this->txt);
    cxInt txtLength = cxStringLength(unicode) / 2;
    cxUInt16 *codeptr = (cxUInt16 *)unicode->strptr.d;
    //init atlas
    cxAtlasSetCapacity(this, txtLength);
    cxAtlasClean(this);
    //compute txt size
    cxUInt16 prev = 0;
    cxVec2f nextpos = cxVec2fv(0, 0);
    cxInt i = 0;
    cxUInt16 *sptr = NULL;
    cxSize2f viewSize = cxViewSize(this);
    viewSize.h = this->font->lineHeight;
    viewSize.w = 0;
    for(i=0,sptr = codeptr; i < txtLength; i++,prev = *sptr,sptr++){
        cxBMPElement pchar = cxBMPFontChar(this->font, *sptr);
        if(pchar == NULL){
            continue;
        }
        cxFloat kerning = cxBMPKerningAmount(this->font, prev, *sptr);
        if(i == 0 && pchar->xadvance < 0){
            viewSize.w -= pchar->xadvance;
        }else if(i == (txtLength - 1) && pchar->box.w > pchar->xadvance){
            viewSize.w += pchar->box.w;
        }else{
            viewSize.w += (pchar->xadvance + kerning);
        }
    }
    cxViewSetSize(this, viewSize);
    //set draw attr
    prev = 0;
    nextpos = cxVec2fv(0, 0);
    for(i=0,sptr = codeptr; i < txtLength; i++,prev = *sptr,sptr++){
        cxBMPElement pchar = cxBMPFontChar(this->font, *sptr);
        if(pchar == NULL){
            continue;
        }
        cxFloat kerning = cxBMPKerningAmount(this->font, prev, *sptr);
        cxSize2f size = cxSize2fv(pchar->box.w, pchar->box.h);
        cxBoxTex2f tex = cxRect4fToBoxTex2f(pchar->box, this->font->scale);
        cxColor4f color = cxViewColor(this);
        cxVec2f pos = cxVec2fv(0,0);
        pos.x = nextpos.x + pchar->xoffset + kerning + size.w/2.0f - viewSize.w/2.0f;
        if(this->isCenter){
            pos.y = 0;
        }else{
            pos.y = nextpos.y + this->font->lineHeight - pchar->yoffset - size.h/2.0f - viewSize.h/2.0f;
        }
        cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, tex, color);
        cxAtlasAppend(this, &bp);
        nextpos.x += (pchar->xadvance + kerning);
    }
    if(this->size > 0){
        cxFloat scale = this->size / viewSize.h;
        cxViewSetFixScale(this, cxVec2fv(scale, scale));
    }
}

static void cxLabelBMPUpdate(cxAny sender)
{
    cxLabelBMP this = sender;
    CX_RETURN(!this->isDirty);
    cxLabelBMPUpdateText(this);
    cxViewAutoResizing(this);
    this->isDirty = false;
}
CX_SETTER_DEF(cxLabelBMP, font)
{
    cxString font = cxJsonString(value, "name");
    if(cxStringOK(font)){
        cxLabelBMPSetFontName(this, font);
    }
    cxFloat size = cxJsonDouble(value, "size", this->size);
    cxLabelBMPSetFontSize(this, size);
}
CX_SETTER_DEF(cxLabelBMP, text)
{
    cxString text = cxJsonToString(value);
    if(cxStringOK(text)){
        cxLabelBMPSetText(this, text);
    }
}
CX_SETTER_DEF(cxLabelBMP, center)
{
    this->isCenter = cxJsonToBool(value, false);
}

CX_OBJECT_TYPE(cxLabelBMP, cxAtlas)
{
    CX_PROPERTY_SETTER(cxLabelBMP, font);
    CX_PROPERTY_SETTER(cxLabelBMP, text);
    CX_PROPERTY_SETTER(cxLabelBMP, center);
}
CX_OBJECT_INIT(cxLabelBMP, cxAtlas)
{
    CX_EVENT_APPEND(this->cxAtlas.cxSprite.cxView.onUpdate, cxLabelBMPUpdate);
    this->isDirty = true;
}
CX_OBJECT_FREE(cxLabelBMP, cxAtlas)
{
    CX_RELEASE(this->font);
    CX_RELEASE(this->txt);
}
CX_OBJECT_TERM(cxLabelBMP, cxAtlas)

cxLabelBMP cxLabelBMPCreate(cxString font,cxString txt)
{
    cxLabelBMP this = CX_CREATE(cxLabelBMP);
    cxLabelBMPSetFontName(this, font);
    cxLabelBMPSetText(this, txt);
    return this;
}

void cxLabelBMPSetFontSize(cxAny pview,cxFloat size)
{
    CX_ASSERT_THIS(pview, cxLabelBMP);
    CX_RETURN(cxFloatEqu(this->size,size));
    this->size = size;
    this->isDirty = true;
}

void cxLabelBMPSetFontName(cxAny pview,cxString font)
{
    CX_ASSERT_THIS(pview, cxLabelBMP);
    cxBMPFont bmpfont = cxEngineLoadBMPFont(cxStringBody(font));
    CX_ASSERT(bmpfont != NULL, "%s font not load",font);
    CX_RETURN(bmpfont == this->font);
    CX_RETAIN_SWAP(this->font, bmpfont);
    this->isDirty = true;
}

void cxLabelBMPSetText(cxAny pview,cxString txt)
{
    CX_ASSERT_THIS(pview, cxLabelBMP);
    CX_RETURN(cxStringEqu(this->txt, txt));
    CX_RETAIN_SWAP(this->txt, txt);
    this->isDirty = true;
}








