//
//  cxFade.c
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxFade.h"

static void cxFadeInit(cxAny pav)
{
    cxFade this = pav;
    CX_ASSERT(this->cxAction.view != NULL, "view not set");
    this->start = this->cxAction.view->color.a;
    this->delta = this->alpha - this->start;
}

static void cxFadeStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxFade this = pav;
    cxFloat alpha = this->start + time * this->delta;
    cxViewSetAlpha(this->cxAction.view, alpha);
}

CX_SETTER_DEF(cxFade, alpha)
{
    this->alpha = cxJsonToDouble(value, this->alpha);
}

CX_OBJECT_TYPE(cxFade, cxAction)
{
    CX_PROPERTY_SETTER(cxFade, alpha);
}
CX_OBJECT_INIT(cxFade, cxAction)
{
    CX_METHOD_SET(this->cxAction.Init, cxFadeInit);
    CX_METHOD_SET(this->cxAction.Step, cxFadeStep);
}
CX_OBJECT_FREE(cxFade, cxAction)
{

}
CX_OBJECT_TERM(cxFade, cxAction)

cxFade cxFadeCreate(cxFloat time,cxFloat alpha)
{
    cxFade this = CX_CREATE(cxFade);
    this->cxAction.time = time;
    this->alpha = alpha;
    return this;
}

