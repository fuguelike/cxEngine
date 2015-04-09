//
//  cxParabola.c
//  cxEngine
//
//  Created by xuhua on 1/9/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxParabola.h"

CX_METHOD_DEF(cxParabola,Step,void,cxFloat dt, cxFloat time)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxVec2f tmp;
    cxVec2f gv;
    cxFloat tv = this->time * this->time / 2.0f;
    tmp = cxVec2fScale1f(this->speed, this->time);
    gv = cxVec2fScale1f(this->gravity, tv);
    tmp = cxVec2fAdd(tmp, gv);
    tmp = cxVec2fAdd(tmp, this->pos);
    cxViewSetPosition(view, tmp);
    this->time += dt;
}
CX_METHOD_DEF(cxParabola,Init,void)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    this->time = 0;
    this->pos = cxViewGetPosition(view);
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxParabola,IsExit,cxBool)
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
    
    CX_METHOD(cxParabola, Init);
    CX_METHOD(cxParabola, Step);
    CX_METHOD(cxParabola, IsExit);
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
