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
    this->startPos = cxViewGetPosition(view);
    this->Angle = cxVec2fRadiansBetween(this->posDelta, this->startPos);
    CX_CALL(this, OnAngle, CX_M(void));
    this->posDelta = cxVec2fSub(this->position, this->startPos);
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxMove,Step,void,cxFloat dt,cxFloat time)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxVec2f currPos = cxViewGetPosition(view);
    cxVec2f npos = cxVec2fScale1f(this->posDelta, time);
    npos = cxVec2fAdd(this->startPos,npos);
    cxFloat angle = cxVec2fRadiansBetween(npos, currPos);
    if(!cxFloatEqu(angle, this->Angle)){
        this->Angle = angle;
        CX_CALL(this, OnAngle, CX_M(void));
    }
    cxViewSetPosition(view, npos);
}
CX_SETTER_DEF(cxMove, position)
{
    this->position = cxJsonToVec2f(value, this->position);
}
CX_METHOD_DEF(cxMove, OnAngle, void)
{
    
}
CX_TYPE(cxMove, cxAction)
{
    CX_SETTER(cxMove, position);
    
    CX_METHOD(cxMove, OnAngle);
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

void cxMoveSetPos(cxAny pav,cxVec2f pos)
{
    CX_ASSERT_THIS(pav, cxMove);
    this->position = pos;
}

cxMove cxMoveCreate(cxFloat time, cxVec2f pos)
{
    cxMove this = CX_CREATE(cxMove);
    this->cxAction.Time = time;
    this->position = pos;
    return this;
}

