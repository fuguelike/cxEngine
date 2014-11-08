//
//  cxFollow.c
//  cxEngine
//  target目标必须存在
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxFollow.h"

static void CX_METHOD(cxFollow,Init)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    CX_ASSERT_VALUE(this->target, cxView, target);
    CX_ASSERT(view != this->target, "target error,can't action'view");
    cxVec2f tpos = cxViewGetPosition(target);
    cxVec2f cpos = cxViewGetPosition(view);
    this->angle = cxVec2fRadiansBetween(tpos, cpos);
    if(CX_CALL(this, IsExit, CX_MT(cxBool))){
        cxActionStop(this);
    }
}

static cxBool CX_METHOD(cxFollow,IsExit)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    CX_ASSERT_VALUE(this->target, cxView, target);
    cxVec2f tpos = cxViewGetPosition(target);
    cxVec2f cpos = cxViewGetPosition(view);
    cxFloat dis = kmVec2DistanceBetween(&tpos, &cpos);
    return dis < 10;
}

static void CX_METHOD(cxFollow, Step,cxFloat dt, cxFloat time)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    CX_ASSERT_VALUE(this->target, cxView, target);
    cxVec2f tpos = cxViewGetPosition(target);
    cxVec2f cpos = cxViewGetPosition(view);
    this->angle = cxVec2fRadiansBetween(tpos, cpos);
    this->speed = this->init;
    cxFloat s = dt * this->speed;
    cxVec2f delta = cxVec2fv(s*cosf(this->angle), s*sinf(this->angle));
    cpos = cxVec2fAdd(cpos, delta);
    if(CX_CALL(this, IsExit, CX_MT(cxBool))){
        cxActionStop(this);
    }
    cxViewSetPosition(view, cpos);
}

cxAny cxFollowTarget(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    return this->target;
}

CX_TYPE(cxFollow, cxAction)
{
    CX_MSET(cxFollow, Init);
    CX_MSET(cxFollow, Step);
    CX_MSET(cxFollow, IsExit);
}
CX_INIT(cxFollow, cxAction)
{
    this->cxAction.Time = -1;
}
CX_FREE(cxFollow, cxAction)
{
    CX_RELEASE(this->target);
}
CX_TERM(cxFollow, cxAction)

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

