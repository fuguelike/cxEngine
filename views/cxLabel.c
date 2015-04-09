//
//  cxLabel.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include <engine/cxUtil.h>
#include <textures/cxTextureCache.h>
#include "cxLabel.h"

CX_SETTER_DEF(cxLabel, align)
{
    cxConstChars str = cxJsonToConstChars(value);
    cxTextAlign align = cxTextAlignNone;
    if(cxConstCharsEqu(str, "left")){
        align = cxTextAlignLeft;
    }else if(cxConstCharsEqu(str, "right")){
        align = cxTextAlignRight;
    }else if(cxConstCharsEqu(str, "center")){
        align = cxTextAlignCenter;
    }else{
        align = cxTextAlignLeft;
    }
    cxLabelSetAlign(this, align);
}
CX_SETTER_DEF(cxLabel, font)
{
    cxStr font = cxJsonStr(value, "name");
    if(font == NULL){
        font = this->font;
    }
    cxFloat size = cxJsonFloat(value, "size", this->attr.size);
    cxColor4f color = cxJsonColor4f(value, "color", this->attr.color);
    cxLabelSetFont(this, font, color, size);
    
    cxBool bold = cxJsonBool(value, "bold", this->attr.boldFont);
    cxLabelSetBold(this, bold);
}
CX_SETTER_DEF(cxLabel, text)
{
    cxStr text = cxJsonToStr(value);
    if(cxStrOK(text)){
        cxLabelSetText(this, text);
    }
}
CX_SETTER_DEF(cxLabel, stroke)
{
    cxFloat width = cxJsonFloat(value, "width", this->attr.strokeWidth);
    cxColor4f color = cxJsonColor4f(value, "color", this->attr.color);
    cxVec2f offset = cxJsonVec2f(value, "offset", cxVec2fv(0, 0));
    cxLabelSetStroke(this, color, width, offset);
}
CX_METHOD_DEF(cxLabel, OnUpdate, void)
{
    if(this->isDirty){
        cxLabelUpdateText(this);
        cxViewAutoResizing(this);
        this->isDirty = false;
    }
    CX_SUPER(cxSprite, this, OnUpdate, CX_M(void));
}
CX_METHOD_DEF(cxLabel, CreateTexture, cxTexture)
{
    return cxTextureCreateText(this->text, this->font, this->attr);
}
CX_TYPE(cxLabel, cxSprite)
{
    CX_SETTER(cxLabel, stroke);
    CX_SETTER(cxLabel, align);
    CX_SETTER(cxLabel, font);
    CX_SETTER(cxLabel, text);
    
    CX_METHOD(cxLabel, CreateTexture);
    CX_METHOD(cxLabel, OnUpdate);
}
CX_INIT(cxLabel, cxSprite)
{
    this->attr.size = 32;
    this->attr.align = cxTextAlignLeft;
    this->attr.color = cxColor4fv(1, 1, 1, 1);
}
CX_FREE(cxLabel, cxSprite)
{
    CX_RELEASE(this->font);
    CX_RELEASE(this->text);
}
CX_TERM(cxLabel, cxSprite)

cxLabel cxLabelCreate(cxStr txt,cxStr font,cxFloat fontsize)
{
    cxLabel this = CX_CREATE(cxLabel);
    cxLabelSetFont(this, font, this->attr.color, fontsize);
    cxLabelSetText(this, txt);
    return this;
}

void cxLabelUpdateText(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLabel);
    cxTexture texture = CX_CALL(this, CreateTexture, CX_M(cxTexture));
    CX_RETURN(texture == NULL);
    cxSpriteSetTexture(this, texture);
    cxViewSetSize(this, texture->size);
}

void cxLabelSetStroke(cxAny pview,cxColor4f color,cxFloat witdh,cxVec2f offset)
{
    CX_ASSERT_THIS(pview, cxLabel);
    if(!cxFloatEqu(this->attr.strokeWidth, witdh)){
        this->attr.strokeWidth = witdh;
        this->isDirty = true;
    }
    if(!cxColor4fEqu(this->attr.strokeColor, color)){
        this->attr.strokeColor = color;
        this->isDirty = true;
    }
    if(!cxVec2fEqu(this->attr.strokeOffset, offset)){
        this->attr.strokeOffset = offset;
        this->isDirty = true;
    }
}

void cxLabelSetFont(cxAny pview,cxStr font, cxColor4f color, cxFloat size)
{
    CX_ASSERT_THIS(pview, cxLabel);
    if(!cxFloatEqu(this->attr.size, size)){
        this->attr.size = size;
        this->isDirty = true;
    }
    if(font != NULL && !cxStrEqu(this->font, font)){
        CX_RETAIN_SWAP(this->font, font);
        this->isDirty = true;
    }
    if(!cxColor4fEqu(this->attr.color, color)){
        this->attr.color = color;
        this->isDirty = true;
    }
}

void cxLabelSetBold(cxAny pview,cxBool bold)
{
    CX_ASSERT_THIS(pview, cxLabel);
    CX_RETURN(this->attr.boldFont == bold);
    this->attr.boldFont = bold;
    this->isDirty = true;
}

void cxLabelSetAlign(cxAny pview,cxTextAlign align)
{
    CX_ASSERT_THIS(pview, cxLabel);
    CX_RETURN(this->attr.align == align);
    this->attr.align = align;
    this->isDirty = true;
}

void cxLabelSetText(cxAny pview,cxStr txt)
{
    CX_ASSERT_THIS(pview, cxLabel);
    CX_RETURN(cxStrEqu(this->text, txt));
    CX_RETAIN_SWAP(this->text, txt);
    this->isDirty = true;
}


