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
    cxInt number = cxStringLength(unicode) / 2;
    cxAtlasSetCapacity(this, number);
    cxAtlasClean(this);
    //get size
    cxSize2f size = cxViewSize(this);
    cxInt width = 0;
    cxInt w = 0;
    cxBMPElement pchar = NULL;
    for(cxInt i=0; i < cxStringLength(unicode);i += 2){
        uint16_t code = *(uint16_t *)&unicode->strptr.d[i];
        pchar = cxBMPFontChar(this->font, code);
        if(pchar == NULL){
            continue;
        }
        w += pchar->xadvance;
        if(width < w){
            width = w;
        }
    }
    size.h = this->font->lineHeight;
    if(pchar->xadvance < pchar->box.w){
        size.w = width + pchar->box.w - pchar->xadvance;
    }else{
        size.w = width;
    }
    cxViewSetSize(this, size);
    //
    cxInt x = -size.w/2.0f;
    cxInt y = 0;
    cxVec2f fontpos = cxVec2fv(0, 0);
    for(cxInt i=0; i < cxStringLength(unicode);i += 2){
        uint16_t code = *(uint16_t *)&unicode->strptr.d[i];
        cxBMPElement pchar = cxBMPFontChar(this->font, code);
        if(pchar == NULL){
            CX_WARN("bmp font char %d not exists",code);
            continue;
        }
        cxInt yoff = this->font->lineHeight / 2.0f - pchar->yoffset;
        fontpos.x = x + pchar->xoffset + pchar->box.w / 2.0f;
        fontpos.y = y + yoff - pchar->box.h / 2.0f;
        cxSize2f size = cxSize2fv(pchar->box.w, pchar->box.h);
        cxBoxTex2f tex = cxRect4fToBoxTex2f(pchar->box, this->font->scale);
        cxColor4f color = cxViewColor(this);
        cxBoxPoint bp = cxAtlasCreateBoxPoint(fontpos, size, tex, color);
        cxAtlasAppend(this, bp);
        x += pchar->xadvance;
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

void cxLabelBMPSetSize(cxAny pview,cxFloat size)
{
    cxLabelBMP this = pview;
    CX_RETURN(cxFloatEqu(this->size,size));
    this->size = size;
    this->isDirty = true;
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








