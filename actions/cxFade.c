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
    CX_ASSERT_THIS(pav, cxFade);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxColor4f color = cxViewGetColor(view);
    this->start = color.a;
    this->delta = this->alpha - this->start;
}

static void cxFadeStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxFade);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxFloat alpha = this->start + time * this->delta;
    cxViewSetAlpha(view, alpha);
}

CX_SETTER_DEF(cxFade, alpha)
{
    this->alpha = cxJsonToDouble(value, this->alpha);
}

CX_TYPE(cxFade, cxAction)
{
    CX_SETTER(cxFade, alpha);
}
CX_INIT(cxFade, cxAction)
{
    CX_SET(cxAction, this, Init, cxFadeInit);
    CX_SET(cxAction, this, Step, cxFadeStep);
}
CX_FREE(cxFade, cxAction)
{

}
CX_TERM(cxFade, cxAction)

cxFade cxFadeCreate(cxFloat time,cxFloat alpha)
{
    cxFade this = CX_CREATE(cxFade);
    this->cxAction.Time = time;
    this->alpha = alpha;
    return this;
}

