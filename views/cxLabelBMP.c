//
//  cxLabelBMP.c
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxIconv.h>
#include "cxLabelBMP.h"

static void cxLabelBMPUpdateText(cxLabelBMP this)
{
    CX_RETURN(!cxStringLength(this->txt));
    
    cxTexture texture = cxBMPFontTexture(this->font, 0);
    cxSpriteSetTexture(this, texture);
    
    cxString unicode = cxIconvConvertUTF8ToUTF16(this->txt);
    cxInt txtLength = cxStringLength(unicode) / 2;
    cxUInt16 *codeptr = (cxUInt16 *)unicode->strptr.d;
    
    cxAtlasSetCapacity(this, txtLength);
    cxAtlasClean(this);
    
    cxUInt16 prev = 0;
    cxVec2f nextpos = cxVec2fv(0, 0);
    cxInt i = 0;
    cxUInt16 *sptr = NULL;
    cxSize2f viewSize = cxViewSize(this);
    viewSize.h = this->font->lineHeight;
    for(i=0,sptr = codeptr; i < txtLength; i++,sptr++,prev = *sptr){
        cxBMPElement pchar = cxBMPFontChar(this->font, *sptr);
        if(pchar == NULL){
            continue;
        }
        cxFloat kerning = cxBMPKerningAmount(this->font, prev, *sptr);
        if(i == 0 && pchar->xadvance < 0){
            nextpos.x -= pchar->xadvance;
        }else if(i == (txtLength - 1) && pchar->box.w > pchar->xadvance){
            nextpos.x += pchar->box.w;
        }else{
            nextpos.x += (pchar->xadvance + kerning);
        }
    }
    viewSize.w = nextpos.x;
    cxViewSetSize(this, viewSize);
    
    prev = 0;
    nextpos = cxVec2fv(0, 0);
    for(i=0,sptr = codeptr; i < txtLength; i++,sptr++,prev = *sptr){
        cxBMPElement pchar = cxBMPFontChar(this->font, *sptr);
        if(pchar == NULL){
            continue;
        }
        cxFloat kerning = cxBMPKerningAmount(this->font, prev, *sptr);
        cxFloat yoffset = this->font->lineHeight - pchar->yoffset;
        cxSize2f size = cxSize2fv(pchar->box.w, pchar->box.h);
        cxBoxTex2f tex = cxRect4fToBoxTex2f(pchar->box, this->font->scale);
        cxColor4f color = cxViewColor(this);
        cxVec2f pos = cxVec2fv(0,0);
        pos.x = nextpos.x + pchar->xoffset + kerning + size.w/2.0f - viewSize.w/2.0f;
        pos.y = nextpos.y + yoffset - size.h/2.0f - viewSize.h/2.0f;
        cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, tex, color);
        cxAtlasAppend(this, bp);
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

CX_OBJECT_INIT(cxLabelBMP, cxAtlas)
{
    CX_EVENT_QUICK(this->super.super.super.onUpdate, cxLabelBMPUpdate);
    this->isDirty = true;
}
CX_OBJECT_FREE(cxLabelBMP, cxAtlas)
{
    CX_RELEASE(this->font);
    CX_RELEASE(this->txt);
}
CX_OBJECT_TERM(cxLabelBMP, cxAtlas)

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








