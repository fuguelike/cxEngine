//
//  cxScale.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxScale.h"

CX_METHOD_DEF(cxScale,Init,void)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->oldScale = cxViewGetScale(view);
    this->delta.x = this->newScale.x - this->oldScale.x;
    this->delta.y = this->newScale.y - this->oldScale.y;
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxScale,Step,void,cxFloat dt,cxFloat time)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxVec2f nscale;
    nscale.x = this->oldScale.x + this->delta.x * time;
    nscale.y = this->oldScale.y + this->delta.y * time;
    cxViewSetScale(view, nscale);
}

CX_SETTER_DEF(cxScale, scale)
{
    this->newScale = cxJsonToVec2f(value, this->newScale);
}

CX_TYPE(cxScale, cxAction)
{
    CX_SETTER(cxScale, scale);
    
    CX_METHOD(cxScale, Init);
    CX_METHOD(cxScale, Step);
}
CX_INIT(cxScale, cxAction)
{

}
CX_FREE(cxScale, cxAction)
{

}
CX_TERM(cxScale, cxAction)

void cxScaleSetScale(cxAny pav,cxVec2f scale)
{
    CX_ASSERT_THIS(pav, cxScale);
    this->newScale = scale;
}

cxScale cxScaleCreate(cxFloat time,cxVec2f scale)
{
    cxScale this = CX_CREATE(cxScale);
    this->cxAction.Time = time;
    this->newScale = scale;
    return this;
}


