//
//  cxTint.c
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTint.h"

static void cxTintInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxTint);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->start = cxViewGetColor(view);
    this->delta.r = this->color.r - this->start.r;
    this->delta.g = this->color.g - this->start.g;
    this->delta.b = this->color.b - this->start.b;
    this->delta.a = this->color.a - this->start.a;
}

static void cxTintStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxTint);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxColor3f color;
    color.r = this->start.r + this->delta.r * time;
    color.g = this->start.g + this->delta.g * time;
    color.b = this->start.b + this->delta.b * time;
    cxFloat alpha = this->start.a + this->delta.a * time;
    cxViewSetColor(view, color);
    cxViewSetAlpha(view, alpha);
}

CX_SETTER_DEF(cxTint, color)
{
    this->color = cxJsonToColor4f(value, this->color);
}

CX_OBJECT_TYPE(cxTint, cxAction)
{
    CX_PROPERTY_SETTER(cxTint, color);
}
CX_OBJECT_INIT(cxTint, cxAction)
{
    CX_SET(cxAction, this, Init, cxTintInit);
    CX_SET(cxAction, this, Step, cxTintStep);
}
CX_OBJECT_FREE(cxTint, cxAction)
{

}
CX_OBJECT_TERM(cxTint, cxAction)

cxTint cxTintCreate(cxFloat time,cxColor4f color)
{
    cxTint this = CX_CREATE(cxTint);
    this->color = color;
    this->cxAction.Time = time;
    return this;
}