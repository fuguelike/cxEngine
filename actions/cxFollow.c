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

static void cxFollowStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxFollow this = pav;
    cxVec2f targetPos = cxViewPosition(this->target);
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

void __cxFollowInitType(cxAny type)
{
    
}

//void __cxFollowInitObject(cxAny object,cxAny json,cxAny hash)
//{
//    cxFollow this = object;
//    cxJson jt = cxJsonObject(json, "target");
//    if(cxJsonIsString(jt)){
//        cxConstChars key = cxJsonToConstChars(jt);
//        cxAny target = cxHashGet(hash, cxHashStrKey(key));
//        cxFollowSetTarget(this, target, false);
//    }else{
//        cxVec2f target = cxJsonVec2f(json, "target", cxVec2fv(0, 0));
//        cxFollowSetVec2f(this, target);
//    }
//    CX_OBJECT_INIT_SUPER(cxAction);
//}

CX_OBJECT_TYPE(cxFollow, cxAction)
{
    
}
CX_OBJECT_INIT(cxFollow, cxAction)
{
    this->super.duration = -1;
    CX_METHOD_SET(this->super.Init, cxFollowInit);
    CX_METHOD_SET(this->super.Step, cxFollowStep);
    CX_METHOD_SET(this->Exit, cxFollowDefaultExit);
}
CX_OBJECT_FREE(cxFollow, cxAction)
{
    CX_RELEASE(this->target);
}
CX_OBJECT_TERM(cxFollow, cxAction)

void cxFollowSetInit(cxAny pav,cxFloat init)
{
    cxFollow this = pav;
    this->init = init;
}

cxFollow cxFollowCreate(cxFloat initSpeed,cxAny target)
{
    cxFollow this = CX_CREATE(cxFollow);
    this->init = initSpeed;
    CX_RETAIN_SWAP(this->target, target);
    return this;
}

