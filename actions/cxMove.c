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
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->prev = this->from = cxViewGetPosition(view);
    this->posDelta = cxVec2fSub(this->to, this->from);
}

void cxMoveSetPos(cxAny pav,cxVec2f pos)
{
    CX_ASSERT_THIS(pav, cxMove);
    this->to = pos;
}

static void cxMoveStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxMove);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxVec2f npos;
    cxVec2f vpos = cxViewGetPosition(view);
    cxVec2f diff = cxVec2fSub(vpos, this->prev);
    this->from = cxVec2fAdd(this->from, diff);
    npos = cxVec2fScale(this->posDelta, time);
    npos = cxVec2fAdd(npos,this->from);
    this->prev = npos;
    cxViewSetPosition(view, npos);
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
    CX_SET(cxAction, this, Init, cxMoveInit);
    CX_SET(cxAction, this, Step, cxMoveStep);
}
CX_OBJECT_FREE(cxMove, cxAction)
{
    
}
CX_OBJECT_TERM(cxMove, cxAction)

cxMove cxMoveCreate(cxFloat time, cxVec2f pos)
{
    cxMove this = CX_CREATE(cxMove);
    this->cxAction.Time = time;
    this->to = pos;
    return this;
}

