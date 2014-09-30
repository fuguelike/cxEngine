//
//  cxFollow.c
//  cxEngine
//  target目标必须存在
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxFollow.h"

void cxFollowInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    CX_ASSERT(this->cxAction.view != this->target, "target error,can't action'view");
    CX_ASSERT(this->cxAction.view != NULL, "view not set");
    CX_ASSERT(this->target != NULL, "target view null");
    cxVec2f tpos = cxViewPosition(this->target);
    cxVec2f cpos = cxViewPosition(this->cxAction.view);
    this->angle = cxVec2fRadiansBetween(tpos, cpos);
    if(CX_METHOD_GET(false, this->IsExit,pav)){
        cxActionStop(pav);
    }
}

static cxBool cxFollowIsExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    cxVec2f tpos = cxViewPosition(this->target);
    cxVec2f cpos = cxViewPosition(this->cxAction.view);
    cxFloat dis = kmVec2DistanceBetween(&tpos, &cpos);
    return dis < 10;
}

static void cxFollowStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxFollow);
    cxVec2f tpos = cxViewPosition(this->target);
    cxVec2f cpos = cxViewPosition(this->cxAction.view);
    this->angle = cxVec2fRadiansBetween(tpos, cpos);
    this->speed = CX_METHOD_GET(this->init, this->Speed,pav,this->cxAction.timeElapsed);
    cxFloat s = dt * this->speed;
    cxVec2f delta = cxVec2fv(s*cosf(this->angle), s*sinf(this->angle));
    kmVec2Add(&cpos, &cpos, &delta);
    if(CX_METHOD_GET(false, this->IsExit,pav)){
        cxActionStop(pav);
    }
    cxViewSetPos(this->cxAction.view, cpos);
}

cxAny cxFollowTarget(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    return this->target;
}

CX_OBJECT_TYPE(cxFollow, cxAction)
{
    
}
CX_OBJECT_INIT(cxFollow, cxAction)
{
    this->cxAction.time = -1;
    SET(cxAction, this, Init, cxFollowInit);
    SET(cxAction, this, Step, cxFollowStep);
    SET(cxFollow, this, IsExit, cxFollowIsExit);
}
CX_OBJECT_FREE(cxFollow, cxAction)
{
    CX_RELEASE(this->target);
}
CX_OBJECT_TERM(cxFollow, cxAction)

void cxFollowSetInit(cxAny pav,cxFloat init)
{
    CX_ASSERT_THIS(pav, cxFollow);
    this->init = init;
}

cxFollow cxFollowCreate(cxFloat initSpeed,cxAny target)
{
    CX_ASSERT_TYPE(target, cxView);
    cxFollow this = CX_CREATE(cxFollow);
    this->init = initSpeed;
    CX_RETAIN_SWAP(this->target, target);
    return this;
}

