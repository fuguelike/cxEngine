//
//  cxSpeed.c
//  engine
//
//  Created by xuhua on 9/4/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxSpeed.h"

static void cxSpeedInit(cxAny pav)
{
    cxSpeed this = pav;
    CX_ASSERT(this->cxAction.view != NULL, "view not set");
    cxVec2f n;
    kmVec2Normalize(&n, &this->speed);
    this->angle = cxVec2fAngle(n);
    this->time = 0;
}

//s = v0*t + a*t*t/2

static void cxSpeedStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxSpeed this = pav;
    CX_ASSERT(this->cxAction.view != NULL, "view not set");
    cxVec2f pos = cxViewPosition(this->cxAction.view);
    cxVec2f np;
    cxVec2f gv;
    cxFloat tv = dt * dt / 2.0f;
    kmVec2Scale(&np, &this->speed, dt);
    kmVec2Scale(&gv, &this->accele, tv);
    kmVec2Add(&np, &np, &gv);
    kmVec2Add(&np, &np, &pos);
    //use and check is set position
    cxBool isExit = CX_METHOD_GET(true, this->UseValue, this, np);
    if(isExit){
        cxActionStop(this);
        return;
    }
    this->time += dt;
}

static cxBool cxSpeedExit(cxAny pav)
{
    cxSpeed this = pav;
    return this->cxAction.time != -1 && this->time >= this->cxAction.time;
}

CX_SETTER_DEF(cxSpeed, accele)
{
    this->accele = cxJsonToVec2f(value, this->accele);
}

CX_SETTER_DEF(cxSpeed, speed)
{
    this->speed = cxJsonToVec2f(value, this->speed);
}

CX_OBJECT_TYPE(cxSpeed, cxAction)
{
    CX_PROPERTY_SETTER(cxSpeed, accele);
    CX_PROPERTY_SETTER(cxSpeed, speed);
}
CX_OBJECT_INIT(cxSpeed, cxAction)
{
    CX_METHOD_SET(this->cxAction.Exit, cxSpeedExit);
    CX_METHOD_SET(this->cxAction.Init, cxSpeedInit);
    CX_METHOD_SET(this->cxAction.Step, cxSpeedStep);
    this->cxAction.time = -1;
}
CX_OBJECT_FREE(cxSpeed, cxAction)
{
    
}
CX_OBJECT_TERM(cxSpeed, cxAction)

cxSpeed cxSpeedCreate(cxVec2f speed,cxVec2f accele)
{
    cxSpeed this = CX_CREATE(cxSpeed);
    this->accele = accele;
    this->speed = speed;
    //compute time Vt = V0 + at^2
    return this;
}