//
//  cxFade.c
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxFade.h"

CX_METHOD_DEF(cxFade,Step,void,cxFloat dt,cxFloat time)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxFloat alpha = this->start + time * this->delta;
    cxViewSetAlpha(view, alpha);
}
CX_METHOD_DEF(cxFade,Init,void)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxColor4f color = cxViewGetColor(view);
    this->start = color.a;
    this->delta = this->alpha - this->start;
}
CX_SETTER_DEF(cxFade, alpha)
{
    this->alpha = cxJsonToDouble(value, this->alpha);
}
CX_TYPE(cxFade, cxAction)
{
    CX_SETTER(cxFade, alpha);
    CX_METHOD(cxFade, Init);
    CX_METHOD(cxFade, Step);
}
CX_INIT(cxFade, cxAction)
{
    
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

