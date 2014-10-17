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
    CX_ASSERT_THIS(sender, cxLabelTTF);
    CX_RETURN(!this->isDirty);
    this->attr.viewSize = cxViewGetSize(this);
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
    cxFloat size = cxJsonDouble(value, "size", this->attr.size);
    cxColor4f color = cxJsonColor4f(value, "color", this->attr.color);
    cxLabelTTFSetFont(this, font, color, size);
}
CX_SETTER_DEF(cxLabelTTF, text)
{
    cxString text = cxJsonToString(value);
    if(cxStringOK(text)){
        cxLabelTTFSetText(this, text);
    }
}
CX_SETTER_DEF(cxLabelTTF, shadow)
{
    cxColor4f color = cxJsonColor4f(value, "color", this->attr.shadowColor);
    cxSize2f offset = cxJsonSize2f(value, "offset", this->attr.shadowOffset);
    cxFloat blur = cxJsonDouble(value, "blur", this->attr.shadowBlur);
    cxLabelTTFSetShadow(this, color, blur, offset);
}
CX_SETTER_DEF(cxLabelTTF, stroke)
{
    cxColor4f color = cxJsonColor4f(value, "color", this->attr.strokeColor);
    cxFloat width = cxJsonDouble(value, "width", this->attr.strokeWidth);
    cxLabelTTFSetStroke(this, color, width);
}

CX_TYPE(cxLabelTTF, cxSprite)
{
    CX_PROPERTY_SETTER(cxLabelTTF, align);
    CX_PROPERTY_SETTER(cxLabelTTF, font);
    CX_PROPERTY_SETTER(cxLabelTTF, text);
    CX_PROPERTY_SETTER(cxLabelTTF, shadow);
    CX_PROPERTY_SETTER(cxLabelTTF, stroke);
}
CX_INIT(cxLabelTTF, cxSprite)
{
    CX_ADD(cxView, this, onUpdate, cxLabelTTFUpdate);
    this->attr.size = 32;
    this->attr.align = cxTextAlignTopLeft;
}
CX_FREE(cxLabelTTF, cxSprite)
{
    CX_RELEASE(this->font);
    CX_RELEASE(this->text);
}
CX_TERM(cxLabelTTF, cxSprite)

cxLabelTTF cxLabelTTFCreate(cxString txt,cxString font,cxFloat fontsize)
{
    cxLabelTTF this = CX_CREATE(cxLabelTTF);
    cxLabelTTFSetFont(this, font, cxColor4fv(1, 1, 1, 1), fontsize);
    cxLabelTTFSetText(this, txt);
    return this;
}

void cxLabelTTFUpdateText(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLabelTTF);
    cxTexture texture = cxTextureFactoryLoadText(this->text, this->font, this->attr);
    CX_RETURN(texture == NULL);
    cxSpriteSetTexture(this, texture);
    cxViewSetSize(this, texture->size);
}

void cxLabelTTFSetFont(cxAny pview,cxString font,cxColor4f color,cxFloat size)
{
    CX_ASSERT_THIS(pview, cxLabelTTF);
    if(!cxFloatEqu(this->attr.size, size)){
        this->attr.size = size;
        this->isDirty = true;
    }
    if(!cxColor4fEqu(this->attr.color, color)){
        this->attr.color = color;
        this->isDirty = true;
    }
    if(!cxStringEqu(this->font, font)){
        CX_RETAIN_SWAP(this->font, font);
        this->isDirty = true;
    }
}

void cxLabelTTFSetAlign(cxAny pview,cxTextAlign align)
{
    CX_ASSERT_THIS(pview, cxLabelTTF);
    CX_RETURN(this->attr.align == align);
    this->attr.align = align;
    this->isDirty = true;
}

void cxLabelTTFSetShadow(cxAny pview,cxColor4f color,cxFloat blur,cxSize2f offset)
{
    CX_ASSERT_THIS(pview, cxLabelTTF);
    if(!cxFloatEqu(this->attr.shadowBlur, blur)){
        this->attr.shadowBlur = blur;
        this->isDirty = true;
    }
    if(!cxColor4fEqu(this->attr.shadowColor, color)){
        this->attr.shadowColor = color;
        this->isDirty = true;
    }
    if(!cxSize2fEqu(this->attr.shadowOffset, offset)){
        this->attr.shadowOffset = offset;
        this->isDirty = true;
    }
}

void cxLabelTTFSetStroke(cxAny pview,cxColor4f color,cxFloat width)
{
    CX_ASSERT_THIS(pview, cxLabelTTF);
    if(!cxFloatEqu(this->attr.strokeWidth, width)){
        this->attr.strokeWidth = width;
        this->isDirty = true;
    }
    if(!cxColor4fEqu(this->attr.strokeColor, color)){
        this->attr.strokeColor = color;
        this->isDirty = true;
    }
}

void cxLabelTTFSetText(cxAny pview,cxString txt)
{
    CX_ASSERT_THIS(pview, cxLabelTTF);
    CX_RETURN(cxStringEqu(this->text, txt));
    CX_RETAIN_SWAP(this->text, txt);
    this->isDirty = true;
}


