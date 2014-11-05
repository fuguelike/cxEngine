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
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->from = this->prev = cxViewGetPosition(view);
}

static void cxBezierStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxBezier);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxVec2f xy;
    xy.x = cxBezier3(0, this->pos1.x, this->pos2.x, this->epos.x, time);
    xy.y = cxBezier3(0, this->pos1.y, this->pos2.y, this->epos.y, time);
    cxVec2f curr = cxViewGetPosition(view);
    cxVec2f diff = cxVec2fSub(curr, this->prev);
    this->from = cxVec2fAdd(this->from, diff);
    cxVec2f npos = cxVec2fAdd(this->from, xy);
    cxViewSetPosition(view, npos);
    this->prev = npos;
}

CX_SETTER_DEF(cxBezier, epos)
{
    this->epos = cxJsonToVec2f(value, this->epos);
}

CX_SETTER_DEF(cxBezier, pos1)
{
    this->pos1 = cxJsonToVec2f(value, this->pos1);
}

CX_SETTER_DEF(cxBezier, pos2)
{
    this->pos2 = cxJsonToVec2f(value, this->pos2);
}

CX_TYPE(cxBezier, cxAction)
{
    CX_SETTER(cxBezier, epos);
    CX_SETTER(cxBezier, pos1);
    CX_SETTER(cxBezier, pos2);
}
CX_INIT(cxBezier, cxAction)
{
    CX_SET(cxAction, this, Init, cxBezierInit);
    CX_SET(cxAction, this, Step, cxBezierStep);
}
CX_FREE(cxBezier, cxAction)
{
    
}
CX_TERM(cxBezier, cxAction)

cxBezier cxBezierCreate(cxFloat time,cxVec2f epos,cxVec2f pos1,cxVec2f pos2)
{
    cxBezier this = CX_CREATE(cxBezier);
    cxActionSetTime(this, time);
    this->epos = epos;
    this->pos1 = pos1;
    this->pos2 = pos2;
    return this;
}








