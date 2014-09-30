//
//  cxBezier.c
//  cxEngine
//
//  Created by xuhua on 8/26/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxBezier.h"

static void cxBezierInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxBezier);
    CX_ASSERT_VALUE(cxActionView(this), cxView, view);
    this->from = this->prev = cxViewGetPosition(view);
}

static void cxBezierStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxBezier);
    CX_ASSERT_VALUE(cxActionView(this), cxView, view);
    cxFloat xa = 0;
    cxFloat xb = this->cpos1.x;
    cxFloat xc = this->cpos2.x;
    cxFloat xd = this->epos.x;
    
    cxFloat ya = 0;
    cxFloat yb = this->cpos1.y;
    cxFloat yc = this->cpos2.y;
    cxFloat yd = this->epos.y;
    
    cxVec2f xy;
    xy.x = cxBezier3(xa, xb, xc, xd, time);
    xy.y = cxBezier3(ya, yb, yc, yd, time);
    
    cxVec2f curr = cxViewGetPosition(view);
    cxVec2f diff;
    kmVec2Subtract(&diff, &curr, &this->prev);
    kmVec2Add(&this->from, &this->from, &diff);
    cxVec2f npos;
    kmVec2Add(&npos, &this->from, &xy);
    cxViewSetPosition(this->cxAction.view, npos);
    this->prev = npos;
}

CX_SETTER_DEF(cxBezier, epos)
{
    this->epos = cxJsonToVec2f(value, this->epos);
}

CX_SETTER_DEF(cxBezier, cpos1)
{
    this->cpos1 = cxJsonToVec2f(value, this->cpos1);
}

CX_SETTER_DEF(cxBezier, cpos2)
{
    this->cpos2 = cxJsonToVec2f(value, this->cpos2);
}

CX_OBJECT_TYPE(cxBezier, cxAction)
{
    CX_PROPERTY_SETTER(cxBezier, epos);
    CX_PROPERTY_SETTER(cxBezier, cpos1);
    CX_PROPERTY_SETTER(cxBezier, cpos2);
}
CX_OBJECT_INIT(cxBezier, cxAction)
{
    SET(cxAction, this, Init, cxBezierInit);
    SET(cxAction, this, Step, cxBezierStep);
}
CX_OBJECT_FREE(cxBezier, cxAction)
{
    
}
CX_OBJECT_TERM(cxBezier, cxAction)

cxBezier cxBezierCreate(cxFloat time,cxVec2f epos,cxVec2f cpos1,cxVec2f cpos2)
{
    cxBezier this = CX_CREATE(cxBezier);
    cxActionSetTime(this, time);
    this->epos = epos;
    this->cpos1 = cpos1;
    this->cpos2 = cpos2;
    return this;
}








