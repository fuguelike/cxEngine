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
    cxTint this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    this->start = cxColor3fv(this->super.view->color.r, this->super.view->color.g, this->super.view->color.b);
    this->delta.r = this->color.r - this->start.r;
    this->delta.g = this->color.g - this->start.g;
    this->delta.b = this->color.b - this->start.b;
}

static void cxTintStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxTint this = pav;
    cxColor3f color;
    color.r = this->start.r + this->delta.r * time;
    color.g = this->start.g + this->delta.g * time;
    color.b = this->start.b + this->delta.b * time;
    cxViewSetColor(this->super.view, color);
}

static void cxTintXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionXMLReadAttr(xmlAction, mAction, reader);
    cxTint this = mAction;
    cxXMLReadFloatsAttr(reader, "cxTint.color", &this->color.r);
}

CX_OBJECT_INIT(cxTint, cxAction)
{
    cxObjectSetXMLReadFunc(this, cxTintXMLReadAttr);
    CX_METHOD_SET(this->super.Init, cxTintInit);
    CX_METHOD_SET(this->super.Step, cxTintStep);
}
CX_OBJECT_FREE(cxTint, cxAction)
{

}
CX_OBJECT_TERM(cxTint, cxAction)

cxTint cxTintCreate(cxFloat duration,cxColor3f color)
{
    cxTint this = CX_CREATE(cxTint);
    this->color = color;
    return this;
}