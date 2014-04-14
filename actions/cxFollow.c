//
//  cxFollow.c
//  cxEngine
//  target目标必须存在
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxFollow.h"

void __cxFollowTypeInit()
{
    
}

cxBool cxFollowDefaultExit(cxAny pav)
{
    cxFollow this = pav;
    cxVec2f tp = cxViewPosition(this->target);
    cxVec2f cp = cxViewPosition(this->super.view);
    cxFloat distance = kmVec2DistanceBetween(&tp, &cp);
    if(distance < this->min){
        return true;
    }
    return false;
}

void cxFollowInit(cxAny pav)
{
    cxFollow this = pav;
    CX_ASSERT(this->super.view != this->target, "target error,can't action'view");
    CX_ASSERT(this->super.view != NULL, "view not set");
    CX_ASSERT(this->target != NULL, "target view null");
    cxSize2f ts = cxViewSize(this->target);
    cxSize2f cs = cxViewSize(this->super.view);
    this->min = CX_MIN(ts.h, ts.w)/2.0f + CX_MIN(cs.h, cs.w)/2.0f;
}

//get target point
static cxVec2f cxFollowTargetPos(cxFollow this)
{
    cxBool isNum = cxObjectIsType(this->target, cxNumberTypeName);
    return isNum ? cxNumberToVec2f(this->target) : cxViewPosition(this->target);
}

static void cxFollowStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxFollow this = pav;
    cxVec2f targetPos = cxFollowTargetPos(this);
    cxVec2f currentPos = cxViewPosition(this->super.view);
    this->angle = cxVec2f2PAngle(targetPos,currentPos);
    this->speed = CX_METHOD_GET(this->init, this->Speed,pav,this->super.durationElapsed);
    cxFloat s = dt * this->speed;
    currentPos.x += (s * cosf(this->angle));
    currentPos.y += (s * sinf(this->angle));
    cxViewSetPos(this->super.view, currentPos);
    this->super.isExit = CX_METHOD_GET(false, this->Exit, this);
}

cxAny cxFollowTarget(cxAny pav)
{
    cxFollow this = pav;
    return this->target;
}

CX_OBJECT_INIT(cxFollow, cxAction)
{
    this->super.duration = -1;
    CX_METHOD_OVERRIDE(this->super.Init, cxFollowInit);
    CX_METHOD_OVERRIDE(this->super.Step, cxFollowStep);
    CX_METHOD_OVERRIDE(this->Exit, cxFollowDefaultExit);
}
CX_OBJECT_FREE(cxFollow, cxAction)
{
    CX_RELEASE(this->target);
    CX_METHOD_RELEASE(this->Exit);
    CX_METHOD_RELEASE(this->Speed);
}
CX_OBJECT_TERM(cxFollow, cxAction)

void cxFollowSetInit(cxAny pav,cxFloat init)
{
    cxFollow this = pav;
    this->init = init;
}

void cxFollowSetVec2f(cxAny pav,cxVec2f pt)
{
    cxNumber num = cxNumberVec2f(pt);
    cxFollowSetTarget(pav, num);
}

void cxFollowSetTarget(cxAny pav,cxAny target)
{
    cxFollow this = pav;
    CX_RETAIN_SWAP(this->target, target);
}

cxFollow cxFollowCreate(cxFloat initSpeed,cxAny target)
{
    cxFollow this = CX_CREATE(cxFollow);
    this->init = initSpeed;
    cxFollowSetTarget(this, target);
    return this;
}

