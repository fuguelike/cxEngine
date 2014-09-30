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
    CX_ASSERT_TYPE(this->cxAction.view, cxView);
    cxAny view = cxActionView(pav);
    cxVec2f n;
    kmVec2Normalize(&n, &this->speed);
    this->angle = cxVec2fAngle(n);
    this->time = 0;
    this->pos = cxViewPosition(view);
}

static void cxParabolaStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxParabola);
    cxView view = cxActionView(pav);
    cxVec2f tmp;
    cxVec2f gv;
    cxFloat tv = this->time * this->time / 2.0f;
    kmVec2Scale(&tmp, &this->speed, this->time);
    kmVec2Scale(&gv, &this->gravity, tv);
    kmVec2Add(&tmp, &tmp, &gv);
    kmVec2Add(&tmp, &tmp, &this->pos);
    cxViewSetPos(view, tmp);
    this->time += dt;
}

static cxBool cxParabolaExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxParabola);
    return this->cxAction.time != -1 && this->time >= this->cxAction.time;
}

CX_SETTER_DEF(cxParabola, speed)
{
    this->speed = cxJsonToVec2f(value, this->speed);
}
CX_SETTER_DEF(cxParabola, gravity)
{
    this->gravity = cxJsonToVec2f(value, this->gravity);
}

CX_OBJECT_TYPE(cxParabola, cxAction)
{
    CX_PROPERTY_SETTER(cxParabola, speed);
    CX_PROPERTY_SETTER(cxParabola, gravity);
}
CX_OBJECT_INIT(cxParabola, cxAction)
{
    SET(cxAction, this, Init, cxParabolaInit);
    SET(cxAction, this, Step, cxParabolaStep);
    SET(cxAction, this, Exit, cxParabolaExit);
    this->gravity = cxVec2fv(0, -1000);
    this->cxAction.time = -1;
}
CX_OBJECT_FREE(cxParabola, cxAction)
{
    
}
CX_OBJECT_TERM(cxParabola, cxAction)

cxParabola cxParabolaCreate(cxVec2f speed)
{
    cxParabola this = CX_CREATE(cxParabola);
    this->speed = speed;
    return this;
}
