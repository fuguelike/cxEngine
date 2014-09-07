//
//  cxFollow.c
//  cxEngine
//  target目标必须存在
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxFollow.h"

cxBool cxFollowDefaultExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    CX_ASSERT_TYPE(this->cxAction.view, cxView);
    cxVec2f tp = cxViewPosition(this->target);
    cxVec2f cp = cxViewPosition(this->cxAction.view);
    cxFloat distance = kmVec2DistanceBetween(&tp, &cp);
    if(distance < this->min){
        return true;
    }
    return false;
}

void cxFollowInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    CX_ASSERT(this->cxAction.view != this->target, "target error,can't action'view");
    CX_ASSERT(this->cxAction.view != NULL, "view not set");
    CX_ASSERT(this->target != NULL, "target view null");
    cxSize2f ts = cxViewSize(this->target);
    cxSize2f cs = cxViewSize(this->cxAction.view);
    this->min = CX_MIN(ts.h, ts.w)/2.0f + CX_MIN(cs.h, cs.w)/2.0f;
}

static void cxFollowStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxFollow);
    cxVec2f targetPos = cxViewPosition(this->target);
    cxVec2f currentPos = cxViewPosition(this->cxAction.view);
    this->angle = cxVec2f2PAngle(targetPos,currentPos);
    this->speed = CX_METHOD_GET(this->init, this->Speed,pav,this->cxAction.timeElapsed);
    cxFloat s = dt * this->speed;
    currentPos.x += (s * cosf(this->angle));
    currentPos.y += (s * sinf(this->angle));
    cxViewSetPos(this->cxAction.view, currentPos);
    this->cxAction.isExit = CX_METHOD_GET(false, this->Exit, this);
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
    CX_METHOD_SET(this->cxAction.Init, cxFollowInit);
    CX_METHOD_SET(this->cxAction.Step, cxFollowStep);
    CX_METHOD_SET(this->Exit, cxFollowDefaultExit);
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
    cxFollow this = CX_CREATE(cxFollow);
    this->init = initSpeed;
    CX_RETAIN_SWAP(this->target, target);
    return this;
}

