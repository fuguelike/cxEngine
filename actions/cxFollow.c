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

static cxBool cxFollowDefaultExit(cxAny pav)
{
    cxFollow this = pav;
    cxVec2f tp = cxViewPosition(this->target);
    cxVec2f cp = cxViewPosition(this->super.view);
    cxFloat distance = kmVec2DistanceBetween(&tp, &cp);
    if(distance < this->minDistance){
        return true;
    }
    return false;
}

static void cxFollowInit(cxAny pav)
{
    cxFollow this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    CX_ASSERT(this->target != NULL, "target view null");
    cxSize2f ts = cxViewSize(this->target);
    cxSize2f cs = cxViewSize(this->super.view);
    this->minDistance = CX_MIN(ts.h, ts.w) / 2.0f + CX_MIN(cs.h, cs.w);
}

//get target point
static cxVec2f cxFollowTargetPos(cxFollow this)
{
    if(!cxObjectIsType(this->target, cxNumberTypeName)){
        return cxViewPosition(this->target);
    }else if(cxNumberIsType(this->target, cxNumberTypeVec2f)){
        return cxNumberToVec2f(this->target);
    }else{
        CX_ASSERT_FALSE("target type error");
        return cxVec2fv(0, 0);
    }
}

static void cxFollowStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxFollow this = pav;
    cxVec2f targetPos = cxFollowTargetPos(this);
    cxVec2f currentPos = cxViewPosition(this->super.view);
    this->angle = cxVec2f2PAngle(targetPos,currentPos);
    this->speed = CX_METHOD_GET(this->initSpeed, this->Speed,pav,this->super.durationElapsed);
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

void cxFollowSetTarget(cxAny pav,cxAny target)
{
    cxFollow this = pav;
    CX_RETAIN_SWAP(this->target, target);
}

cxFollow cxFollowCreate(cxFloat initSpeed,cxAny target)
{
    cxFollow this = CX_CREATE(cxFollow);
    this->super.duration = -1;
    this->initSpeed = initSpeed;
    cxFollowSetTarget(this, target);
    return this;
}

