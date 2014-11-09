//
//  cxMove.c
//  cxEngine
//
//  Created by xuhua on 10/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxMove.h"

CX_METHOD_DEF(cxMove,Init,void)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->prev = this->from = cxViewGetPosition(view);
    this->posDelta = cxVec2fSub(this->to, this->from);
}

void cxMoveSetPos(cxAny pav,cxVec2f pos)
{
    CX_ASSERT_THIS(pav, cxMove);
    this->to = pos;
}

CX_METHOD_DEF(cxMove,Step,void,cxFloat dt,cxFloat time)
{
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

CX_TYPE(cxMove, cxAction)
{
    CX_SETTER(cxMove, to);
    
    CX_METHOD(cxMove, Init);
    CX_METHOD(cxMove, Step);
}
CX_INIT(cxMove, cxAction)
{
    
}
CX_FREE(cxMove, cxAction)
{
    
}
CX_TERM(cxMove, cxAction)

cxMove cxMoveCreate(cxFloat time, cxVec2f pos)
{
    cxMove this = CX_CREATE(cxMove);
    this->cxAction.Time = time;
    this->to = pos;
    return this;
}

