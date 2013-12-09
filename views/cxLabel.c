//
//  cxLabel.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxUtil.h>
#include <textures/cxTextureFactory.h>
#include "cxLabel.h"

void cxLabelXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxSpriteXMLReadAttr(xmlView, mView, reader);
    cxLabel this = mView;
    //set font
    cxString font = cxXMLReadLangStringAttr(reader, "cxLabel.font");
    cxLabelSetFont(mView, font);
    //set text
    cxString text = cxXMLReadLangStringAttr(reader, "cxLabel.text");
    cxLabelSetText(mView, text);
    //set fontsize
    cxLabelSetFontSize(mView, cxXMLReadFloatAttr(reader, "cxLabel.size", this->attr.size));
    //set fontbold
    cxLabelSetFontBold(mView, cxXMLReadBoolAttr(reader, "cxLabel.bold", this->attr.bold));
}

static void cxLabelUpdate(cxEvent *event)
{
    cxLabel this = event->sender;
    CX_RETURN(!this->isDirty);
    cxLabelUpdateText(this);
    this->isDirty = false;
}

CX_OBJECT_INIT(cxLabel, cxSprite)
{
    cxObjectSetXMLReadFunc(this, cxLabelXMLReadAttr);
    cxViewOnUpdate(this, cxLabelUpdate);
    this->attr.size = 32;
    cxSpriteSetShader(this, cxShaderAlphaKey);
}
CX_OBJECT_FREE(cxLabel, cxSprite)
{
    CX_RELEASE(this->font);
    CX_RELEASE(this->text);
}
CX_OBJECT_TERM(cxLabel, cxSprite)

cxLabel cxLabelCreate(cxString txt,cxString font,cxFloat fontsize)
{
    cxLabel this = CX_CREATE(cxLabel);
    cxLabelSetFont(this, font);
    cxLabelSetText(this, txt);
    cxLabelSetFontSize(this, fontsize);
    return this;
}

void cxLabelUpdateText(cxAny pview)
{
    cxLabel this = pview;
    cxTexture texture = cxTextureFactoryLoadText(this->text, this->font, this->attr);
    CX_RETURN(texture == NULL);
    cxSpriteSetTexture(this, texture);
    cxViewSetSize(this, texture->size);
}

void cxLabelSetFontBold(cxAny pview,cxBool bold)
{
    cxLabel this = pview;
    CX_RETURN(this->attr.bold == bold);
    this->attr.bold = bold;
    this->isDirty = true;
}

void cxLabelSetFontSize(cxAny pview,cxFloat fontsize)
{
    cxLabel this = pview;
    CX_RETURN(kmAlmostEqual(this->attr.size, fontsize));
    this->attr.size = fontsize;
    this->isDirty = true;
}

void cxLabelSetFont(cxAny pview,cxString font)
{
    cxLabel this = pview;
    CX_RETURN(cxStringEqu(this->font, font));
    CX_RETAIN_SWAP(this->font, font);
    this->isDirty = true;
}

void cxLabelSetText(cxAny pview,cxString txt)
{
    cxLabel this = pview;
    CX_RETURN(cxStringEqu(this->text, txt));
    CX_RETAIN_SWAP(this->text, txt);
    this->isDirty = true;
}


