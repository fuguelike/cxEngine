//
//  cxParabola.c
//  cxEngine
//
//  Created by xuhua on 1/9/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxParabola.h"

static void cxParabolaInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxParabola);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->time = 0;
    this->pos = cxViewGetPosition(view);
}

static void cxParabolaStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxParabola);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxVec2f tmp;
    cxVec2f gv;
    cxFloat tv = this->time * this->time / 2.0f;
    tmp = cxVec2fScale(this->speed, this->time);
    gv = cxVec2fScale(this->gravity, tv);
    tmp = cxVec2fAdd(tmp, gv);
    tmp = cxVec2fAdd(tmp, this->pos);
    cxViewSetPosition(view, tmp);
    this->time += dt;
}

static cxBool cxParabolaExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxParabola);
    cxFloat time = cxActionGetTime(this);
    return time != -1 && this->time >= time;
}

CX_SETTER_DEF(cxParabola, speed)
{
    this->speed = cxJsonToVec2f(value, this->speed);
}
CX_SETTER_DEF(cxParabola, gravity)
{
    this->gravity = cxJsonToVec2f(value, this->gravity);
}

CX_TYPE(cxParabola, cxAction)
{
    CX_PROPERTY_SETTER(cxParabola, speed);
    CX_PROPERTY_SETTER(cxParabola, gravity);
}
CX_INIT(cxParabola, cxAction)
{
    CX_SET(cxAction, this, Init, cxParabolaInit);
    CX_SET(cxAction, this, Step, cxParabolaStep);
    CX_SET(cxAction, this, Exit, cxParabolaExit);
    this->gravity = cxVec2fv(0, -1000);
    this->cxAction.Time = -1;
}
CX_FREE(cxParabola, cxAction)
{
    
}
CX_TERM(cxParabola, cxAction)

cxParabola cxParabolaCreate(cxVec2f speed)
{
    cxParabola this = CX_CREATE(cxParabola);
    this->speed = speed;
    return this;
}
