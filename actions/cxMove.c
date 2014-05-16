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
    cxMove this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    this->prev = this->from = this->super.view->position;
    kmVec2Subtract(&this->posDelta, &this->to, &this->from);
}

void cxMoveSetPos(cxAny pav,cxVec2f pos)
{
    cxMove this = pav;
    this->to = pos;
}

static void cxMoveStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxMove this = pav;
    cxVec2f npos;
    cxVec2f diff;
    kmVec2Subtract(&diff, &this->super.view->position, &this->prev);
    kmVec2Add(&this->from, &this->from, &diff);
    kmVec2Scale(&npos, &this->posDelta, time);
    kmVec2Add(&npos, &this->from, &npos);
    this->prev = npos;
    cxViewSetPos(this->super.view, npos);
}

void __cxMoveInitType(cxAny type)
{
    
}

//void __cxMoveInitObject(cxAny object,cxAny json,cxAny hash)
//{
//    cxMove this = object;
//    this->to = cxJsonVec2f(json, "to", this->to);
//}

CX_OBJECT_TYPE(cxMove, cxAction)
{
    
}
CX_OBJECT_INIT(cxMove, cxAction)
{
    CX_METHOD_SET(this->super.Init, cxMoveInit);
    CX_METHOD_SET(this->super.Step, cxMoveStep);
}
CX_OBJECT_FREE(cxMove, cxAction)
{
    
}
CX_OBJECT_TERM(cxMove, cxAction)

cxMove cxMoveCreate(cxFloat dutation, cxVec2f pos)
{
    cxMove this = CX_CREATE(cxMove);
    this->super.duration = dutation;
    this->to = pos;
    return this;
}

