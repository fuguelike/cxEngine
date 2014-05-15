//
//  cxLabelTTF.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxUtil.h>
#include <textures/cxTextureFactory.h>
#include "cxLabelTTF.h"

static void cxLabelTTFUpdate(cxEvent *event)
{
    cxLabelTTF this = event->sender;
    CX_RETURN(!this->isDirty);
    cxLabelTTFUpdateText(this);
    this->isDirty = false;
}

void __cxLabelTTFInitType(cxAny type)
{
    
}

//void __cxLabelTTFInitObject(cxAny object,cxAny json,cxAny hash)
//{
//    cxLabelTTF this = object;
//    cxString font = cxJsonString(json, "font.name");
//    if(cxStringOK(font)){
//        cxLabelTTFSetFont(this, font);
//    }
//    cxString text = cxJsonString(json, "text");
//    if(cxStringOK(text)){
//        cxLabelTTFSetText(this, text);
//    }
//    cxLabelTTFSetFontBold(this, cxJsonBool(json, "font.bold", this->attr.bold));
//    cxLabelTTFSetFontSize(this, cxJsonDouble(json, "font.size", this->attr.size));
//    CX_OBJECT_INIT_SUPER(cxSprite);
//}

CX_OBJECT_INIT(cxLabelTTF, cxSprite)
{
    CX_EVENT_QUICK(this->super.super.onUpdate,cxLabelTTFUpdate);
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


