//
//  cxMove.c
//  cxEngine
//
//  Created by xuhua on 10/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxMove.h"

static void cxMoveInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxMove);
    CX_ASSERT_VALUE(cxActionView(this), cxView, view);
    this->prev = this->from = cxViewGetPosition(view);
    kmVec2Subtract(&this->posDelta, &this->to, &this->from);
}

void cxMoveSetPos(cxAny pav,cxVec2f pos)
{
    CX_ASSERT_THIS(pav, cxMove);
    this->to = pos;
}

static void cxMoveStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxMove);
    CX_ASSERT_VALUE(cxActionView(this), cxView, view);
    cxVec2f npos;
    cxVec2f diff;
    cxVec2f vpos = cxViewGetPosition(view);
    kmVec2Subtract(&diff, &vpos, &this->prev);
    kmVec2Add(&this->from, &this->from, &diff);
    kmVec2Scale(&npos, &this->posDelta, time);
    kmVec2Add(&npos, &this->from, &npos);
    this->prev = npos;
    cxViewSetPosition(this->cxAction.view, npos);
}

CX_SETTER_DEF(cxMove, to)
{
    this->to = cxJsonToVec2f(value, this->to);
}

CX_OBJECT_TYPE(cxMove, cxAction)
{
    CX_PROPERTY_SETTER(cxMove, to);
}
CX_OBJECT_INIT(cxMove, cxAction)
{
    SET(cxAction, this, Init, cxMoveInit);
    SET(cxAction, this, Step, cxMoveStep);
}
CX_OBJECT_FREE(cxMove, cxAction)
{
    
}
CX_OBJECT_TERM(cxMove, cxAction)

cxMove cxMoveCreate(cxFloat time, cxVec2f pos)
{
    cxMove this = CX_CREATE(cxMove);
    this->cxAction.time = time;
    this->to = pos;
    return this;
}

