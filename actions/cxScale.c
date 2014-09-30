//
//  cxScale.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxScale.h"

static void cxScaleInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxScale);
    CX_ASSERT_TYPE(this->cxAction.view, cxView);
    this->oldScale = this->cxAction.view->scale;
    this->delta.x = this->newScale.x - this->oldScale.x;
    this->delta.y = this->newScale.y - this->oldScale.y;
}

static void cxScaleStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxScale);
    cxVec2f nscale;
    nscale.x = this->oldScale.x + this->delta.x * time;
    nscale.y = this->oldScale.y + this->delta.y * time;
    cxViewSetScale(this->cxAction.view, nscale);
}

CX_SETTER_DEF(cxScale, scale)
{
    this->newScale = cxJsonToVec2f(value, this->newScale);
}

CX_OBJECT_TYPE(cxScale, cxAction)
{
    CX_PROPERTY_SETTER(cxScale, scale);
}
CX_OBJECT_INIT(cxScale, cxAction)
{
    SET(cxAction, this, Init, cxScaleInit);
    SET(cxAction, this, Step, cxScaleStep);
}
CX_OBJECT_FREE(cxScale, cxAction)
{

}
CX_OBJECT_TERM(cxScale, cxAction)

void cxScaleSetScale(cxAny pav,cxVec2f scale)
{
    CX_ASSERT_THIS(pav, cxScale);
    this->newScale = scale;
}

cxScale cxScaleCreate(cxFloat time,cxVec2f scale)
{
    cxScale this = CX_CREATE(cxScale);
    this->cxAction.time = time;
    this->newScale = scale;
    return this;
}


