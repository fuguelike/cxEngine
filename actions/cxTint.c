//
//  cxTint.c
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTint.h"

void __cxTintTypeInit()
{
    
}

static void cxTintInit(cxAny pav)
{
    cxTint this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    this->start = this->super.view->color;
    this->delta.r = this->color.r - this->start.r;
    this->delta.g = this->color.g - this->start.g;
    this->delta.b = this->color.b - this->start.b;
    this->delta.a = this->color.a - this->start.a;
}

static void cxTintStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxTint this = pav;
    cxColor3f color;
    color.r = this->start.r + this->delta.r * time;
    color.g = this->start.g + this->delta.g * time;
    color.b = this->start.b + this->delta.b * time;
    cxFloat alhpa = this->start.a + this->delta.a * time;
    cxViewSetColor(this->super.view, color);
    cxViewSetAlpha(this->super.view, alhpa);
}

void __cxTintInitType(cxAny type)
{
    
}

CX_OBJECT_TYPE(cxTint, cxAction)
{
    
}
CX_OBJECT_INIT(cxTint, cxAction)
{
    CX_METHOD_SET(this->super.Init, cxTintInit);
    CX_METHOD_SET(this->super.Step, cxTintStep);
}
CX_OBJECT_FREE(cxTint, cxAction)
{

}
CX_OBJECT_TERM(cxTint, cxAction)

cxTint cxTintCreate(cxFloat duration,cxColor4f color)
{
    cxTint this = CX_CREATE(cxTint);
    this->color = color;
    return this;
}