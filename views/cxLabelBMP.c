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
        pos.y = nextpos.y + this->font->lineHeight - pchar->yoffset - size.h/2.0f - viewSize.h/2.0f;
        cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, tex, color);
        cxAtlasAppend(this, &bp);
        nextpos.x += (pchar->xadvance + kerning);
    }
}

static void cxLabelBMPUpdate(cxEvent *event)
{
    cxLabelBMP this = event->sender;
    CX_RETURN(!this->isDirty);
    cxLabelBMPUpdateText(this);
    this->isDirty = false;
}
CX_SETTER_DEF(cxLabelBMP, font)
{
    cxString font = cxJsonString(value, "name");
    if(cxStringOK(font)){
        cxLabelBMPSetFont(this, font);
    }
}
CX_SETTER_DEF(cxLabelBMP, text)
{
    cxString text = cxJsonToString(value);
    if(cxStringOK(text)){
        cxLabelBMPSetText(this, text);
    }
}

CX_OBJECT_TYPE(cxLabelBMP, cxAtlas)
{
    CX_PROPERTY_SETTER(this, cxLabelBMP, font);
    CX_PROPERTY_SETTER(this, cxLabelBMP, text);
}
CX_OBJECT_INIT(cxLabelBMP, cxAtlas)
{
    CX_EVENT_APPEND(this->super.super.super.onUpdate, cxLabelBMPUpdate);
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
    cxLabelBMPSetFont(this, font);
    cxLabelBMPSetText(this, txt);
    return this;
}

void cxLabelBMPSetFont(cxAny pview,cxString font)
{
    cxLabelBMP this = pview;
    cxBMPFont bmpfont = cxEngineLoadBMPFont(cxStringBody(font));
    CX_ASSERT(bmpfont != NULL, "%s font not load",font);
    CX_RETURN(bmpfont == this->font);
    CX_RETAIN_SWAP(this->font, bmpfont);
    this->isDirty = true;
}

void cxLabelBMPSetText(cxAny pview,cxString txt)
{
    cxLabelBMP this = pview;
    CX_RETURN(cxStringEqu(this->txt, txt));
    CX_RETAIN_SWAP(this->txt, txt);
    this->isDirty = true;
}








