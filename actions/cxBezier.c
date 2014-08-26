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
    cxBezier this = pav;
    CX_ASSERT(this->cxAction.view != NULL, "view not set");
    this->from = this->prev = this->cxAction.view->position;
}

static void cxBezierStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxBezier this = pav;
    
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
    
    cxVec2f curr = this->cxAction.view->position;
    cxVec2f diff;
    kmVec2Subtract(&diff, &curr, &this->prev);
    kmVec2Add(&this->from, &this->from, &diff);
    cxVec2f npos;
    kmVec2Add(&npos, &this->from, &xy);
    cxViewSetPos(this->cxAction.view, npos);
    this->prev = npos;
}

CX_OBJECT_TYPE(cxBezier, cxAction)
{
    
}
CX_OBJECT_INIT(cxBezier, cxAction)
{
    CX_METHOD_SET(this->cxAction.Init, cxBezierInit);
    CX_METHOD_SET(this->cxAction.Step, cxBezierStep);
}
CX_OBJECT_FREE(cxBezier, cxAction)
{
    
}
CX_OBJECT_TERM(cxBezier, cxAction)

cxBezier cxBezierCreate(cxFloat duration,cxVec2f epos,cxVec2f cpos1,cxVec2f cpos2)
{
    cxBezier this = CX_CREATE(cxBezier);
    cxActionSetDuration(this, duration);
    this->epos = epos;
    this->cpos1 = cpos1;
    this->cpos2 = cpos2;
    return this;
}








