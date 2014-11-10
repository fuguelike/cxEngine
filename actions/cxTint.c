//
//  cxTint.c
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTint.h"

CX_METHOD_DEF(cxTint,Init,void)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->start = cxViewGetColor(view);
    this->delta.r = this->color.r - this->start.r;
    this->delta.g = this->color.g - this->start.g;
    this->delta.b = this->color.b - this->start.b;
    this->delta.a = this->color.a - this->start.a;
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxTint,Step,void,cxFloat dt,cxFloat time)
{
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

CX_TYPE(cxTint, cxAction)
{
    CX_SETTER(cxTint, color);
    CX_METHOD(cxTint, Init);
    CX_METHOD(cxTint, Step);
}
CX_INIT(cxTint, cxAction)
{
}
CX_FREE(cxTint, cxAction)
{

}
CX_TERM(cxTint, cxAction)

cxTint cxTintCreate(cxFloat time,cxColor4f color)
{
    cxTint this = CX_CREATE(cxTint);
    this->color = color;
    this->cxAction.Time = time;
    return this;
}