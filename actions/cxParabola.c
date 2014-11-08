//
//  cxParabola.c
//  cxEngine
//
//  Created by xuhua on 1/9/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxParabola.h"

static void CX_METHOD(cxParabola, Step,cxFloat dt, cxFloat time)
{
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
static void CX_METHOD(cxParabola,Init)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->time = 0;
    this->pos = cxViewGetPosition(view);
}
static cxBool CX_METHOD(cxParabola,Exit)
{
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
    CX_SETTER(cxParabola, speed);
    CX_SETTER(cxParabola, gravity);
    
    CX_MSET(cxParabola, Init);
    CX_MSET(cxParabola, Step);
    CX_MSET(cxParabola, Exit);
}
CX_INIT(cxParabola, cxAction)
{
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
