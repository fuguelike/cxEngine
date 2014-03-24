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
        CX_LOGGER("exit %f",this->minDistance);
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

static void cxFollowStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxFollow this = pav;
    cxVec2f targetPos = cxViewPosition(this->target);
    cxVec2f currentPos = cxViewPosition(this->super.view);
    cxFloat angle = atan2f(targetPos.y - currentPos.y, targetPos.x - currentPos.x);
    cxFloat s = dt * this->speed;
    currentPos.x += (s * cosf(angle));
    currentPos.y += (s * sinf(angle));
    cxViewSetPos(this->super.view, currentPos);
    this->super.isExit = CX_METHOD_FIRE(false, this->FollowExit, this);
}

CX_OBJECT_INIT(cxFollow, cxAction)
{
    CX_METHOD_OVERRIDE(this->super.Init, cxFollowInit);
    CX_METHOD_OVERRIDE(this->super.Step, cxFollowStep);
    CX_METHOD_OVERRIDE(this->FollowExit, cxFollowDefaultExit);
}
CX_OBJECT_FREE(cxFollow, cxAction)
{
    CX_METHOD_RELEASE(this->FollowExit);
}
CX_OBJECT_TERM(cxFollow, cxAction)

cxFollow cxFollowCreate(cxFloat speed,cxAny target)
{
    cxFollow this = CX_CREATE(cxFollow);
    this->super.duration = -1;
    this->speed = speed;
    this->target = target;
    return this;
}

