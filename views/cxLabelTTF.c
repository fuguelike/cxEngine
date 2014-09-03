//
//  cxLabelTTF.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include <engine/cxUtil.h>
#include <textures/cxTextureFactory.h>
#include "cxLabelTTF.h"

static void cxLabelTTFUpdate(cxAny sender)
{
    cxLabelTTF this = sender;
    CX_RETURN(!this->isDirty);
    this->attr.viewSize = cxViewSize(this);
    cxLabelTTFUpdateText(this);
    cxViewAutoResizing(this);
    this->isDirty = false;
}
CX_SETTER_DEF(cxLabelTTF, align)
{
    cxConstChars str = cxJsonToConstChars(value);
    cxBool top = cxConstCharsHas(str, "top");
    cxBool left = cxConstCharsHas(str, "left");
    cxBool right = cxConstCharsHas(str, "right");
    cxBool bottom = cxConstCharsHas(str, "bottom");
    cxBool center = cxConstCharsHas(str, "center");
    cxTextAlign align = cxTextAlignTopLeft;
    if(top && right){
        align = cxTextAlignTopRight;
    }else if(top && left){
        align = cxTextAlignTopLeft;
    }else if(top){
        align = cxTextAlignTop;
    }else if(left){
        align = cxTextAlignLeft;
    }
    if(bottom && left){
        align = cxTextAlignBottomLeft;
    }else if(bottom && right){
        align = cxTextAlignBottomRight;
    }else if(bottom){
        align = cxTextAlignBottom;
    }else if(right){
        align = cxTextAlignRight;
    }
    if(center){
        align = cxTextAlignCenter;
    }
    cxLabelTTFSetAlign(this, align);
}
CX_SETTER_DEF(cxLabelTTF, font)
{
    cxString font = cxJsonString(value, "name");
    if(cxStringOK(font)){
        cxLabelTTFSetFontName(this, font);
    }
    cxFloat size = cxJsonDouble(value, "size", this->attr.size);
    cxLabelTTFSetFontSize(this, size);
}
CX_SETTER_DEF(cxLabelTTF, text)
{
    cxString text = cxJsonToString(value);
    if(cxStringOK(text)){
        cxLabelTTFSetText(this, text);
    }
}

CX_OBJECT_TYPE(cxLabelTTF, cxSprite)
{
    CX_PROPERTY_SETTER(cxLabelTTF, align);
    CX_PROPERTY_SETTER(cxLabelTTF, font);
    CX_PROPERTY_SETTER(cxLabelTTF, text);
}
CX_OBJECT_INIT(cxLabelTTF, cxSprite)
{
    CX_EVENT_APPEND(this->cxSprite.cxView.onUpdate,cxLabelTTFUpdate);
    this->attr.size = 32;
    this->attr.align = cxTextAlignTopLeft;
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
    cxLabelTTFSetFontName(this, font);
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

void cxLabelTTFSetFontSize(cxAny pview,cxFloat size)
{
    cxLabelTTF this = pview;
    CX_RETURN(kmAlmostEqual(this->attr.size, size));
    this->attr.size = size;
    this->isDirty = true;
}

void cxLabelTTFSetAlign(cxAny pview,cxTextAlign align)
{
    cxLabelTTF this = pview;
    CX_RETURN(this->attr.align == align);
    this->attr.align = align;
    this->isDirty = true;
}

void cxLabelTTFSetFontName(cxAny pview,cxString font)
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


