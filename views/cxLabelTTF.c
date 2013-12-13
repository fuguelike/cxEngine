//
//  cxLabelTTF.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxUtil.h>
#include <core/cxViewRoot.h>
#include <textures/cxTextureFactory.h>
#include "cxLabelTTF.h"

void cxLabelTTFReadAttr(cxAny rootView,cxAny mView, xmlTextReaderPtr reader)
{
    cxViewRoot root = rootView;
    cxSpriteReadAttr(rootView, mView, reader);
    cxLabelTTF this = mView;
    //set font
    cxString font = cxXMLReadStringAttr(reader, root->functions, "cxLabelTTF.font");
    cxLabelTTFSetFont(mView, font);
    //set text
    cxString text = cxXMLReadStringAttr(reader, root->functions, "cxLabelTTF.text");
    cxLabelTTFSetText(mView, text);
    //set fontsize
    cxLabelTTFSetFontSize(mView, cxXMLReadFloatAttr(reader, root->functions, "cxLabelTTF.size", this->attr.size));
    //set fontbold
    cxLabelTTFSetFontBold(mView, cxXMLReadBoolAttr(reader,root->functions, "cxLabelTTF.bold", this->attr.bold));
}

static void cxLabelTTFUpdate(cxEvent *event)
{
    cxLabelTTF this = event->sender;
    CX_RETURN(!this->isDirty);
    cxLabelTTFUpdateText(this);
    this->isDirty = false;
}

CX_OBJECT_INIT(cxLabelTTF, cxSprite)
{
    cxObjectSetReadAttrFunc(this, cxLabelTTFReadAttr);
    cxViewOnUpdate(this, cxLabelTTFUpdate);
    this->attr.size = 32;
    cxSpriteSetShader(this, cxShaderAlphaKey);
}
CX_OBJECT_FREE(cxLabelTTF, cxSprite)
{
    CX_RELEASE(this->font);
    CX_RELEASE(this->text);
}
CX_OBJECT_TERM(cxLabelTTF, cxSprite)

cxLabelTTF cxLabelTTFCreate(cxString txt,cxString font,cxFloat fontsize)
{
    cxLabelTTF this = CX_CREATE(cxLabelTTF);
    cxLabelTTFSetFont(this, font);
    cxLabelTTFSetText(this, txt);
    cxLabelTTFSetFontSize(this, fontsize);
    return this;
}

void cxLabelTTFUpdateText(cxAny pview)
{
    cxLabelTTF this = pview;
    cxTexture texture = cxTextureFactoryLoadText(this->text, this->font, this->attr);
    CX_RETURN(texture == NULL);
    cxSpriteSetTexture(this, texture);
    cxViewSetSize(this, texture->size);
}

void cxLabelTTFSetFontBold(cxAny pview,cxBool bold)
{
    cxLabelTTF this = pview;
    CX_RETURN(this->attr.bold == bold);
    this->attr.bold = bold;
    this->isDirty = true;
}

void cxLabelTTFSetFontSize(cxAny pview,cxFloat fontsize)
{
    cxLabelTTF this = pview;
    CX_RETURN(kmAlmostEqual(this->attr.size, fontsize));
    this->attr.size = fontsize;
    this->isDirty = true;
}

void cxLabelTTFSetFont(cxAny pview,cxString font)
{
    cxLabelTTF this = pview;
    CX_RETURN(cxStringEqu(this->font, font));
    CX_RETAIN_SWAP(this->font, font);
    this->isDirty = true;
}

void cxLabelTTFSetText(cxAny pview,cxString txt)
{
    cxLabelTTF this = pview;
    CX_RETURN(cxStringEqu(this->text, txt));
    CX_RETAIN_SWAP(this->text, txt);
    this->isDirty = true;
}


