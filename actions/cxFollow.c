//
//  cxFollow.c
//  cxEngine
//  target目标必须存在
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxFollow.h"

CX_METHOD_DEF(cxFollow,Init,void)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxAny target = cxFollowTarget(this);
    if(target == NULL || !CX_METHOD_HAS(target, FollowPos)){
        cxActionStop(this);
        return;
    }
    this->opos = cxViewGetPosition(view);
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxFollow, OnAngle, void)
{
    
}
CX_METHOD_DEF(cxFollow,Step,void,cxFloat dt, cxFloat time)
{
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxAny target = cxFollowTarget(this);
    if(target == NULL){
        cxActionStop(this);
        return;
    }
    cxFloat s = dt * this->speed;
    cxVec2f tpos = CX_CALL(target, FollowPos, CX_M(cxVec2f));
    cxVec2f cpos = cxViewGetPosition(view);
    cxFloat dis = cxVec2fDistanceBetween(tpos, cpos);
    if(cxFloatEqu(dis, s) || dis < s){
        cxViewSetPosition(view, tpos);
        cxActionStop(this);
        return;
    }
    cxFloat angle = cxVec2fRadiansBetween(tpos, cpos);
    if(!cxFloatEqu(this->Angle, angle)){
        this->Angle = angle;
        CX_CALL(this, OnAngle, CX_M(void));
    }
    cxVec2f delta = cxVec2fv(s*cosf(this->Angle), s*sinf(this->Angle));
    cpos = cxVec2fAdd(cpos, delta);
    cxViewSetPosition(view, cpos);
}

cxAny cxFollowTarget(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxHash bindes = cxViewGetBindes(view);
    CX_HASH_FOREACH(bindes, ele, tmp){
        cxAny pv = cxHashKeyToAny(ele);
        if(pv == this->target){
            return pv;
        }
    }
    return NULL;
}

CX_TYPE(cxFollow, cxAction)
{
    CX_METHOD(cxFollow, OnAngle);
    CX_METHOD(cxFollow, Init);
    CX_METHOD(cxFollow, Step);
}
CX_INIT(cxFollow, cxAction)
{
    this->Angle = INFINITY;
    cxActionSetTime(this, CX_FOREVER);
}
CX_FREE(cxFollow, cxAction)
{
    
}
CX_TERM(cxFollow, cxAction)

void cxFollowInit(cxAny pav,cxAny pview,cxFloat speed,cxAny target)
{
    CX_ASSERT_THIS(pav, cxFollow);
    CX_ASSERT_TYPE(target, cxView);
    this->speed = speed;
    this->target = target;
    cxViewBind(pview, target, NULL);
}

cxFollow cxFollowCreate(cxFloat speed,cxAny pview, cxAny target)
{
    CX_ASSERT_TYPE(target, cxView);
    cxFollow this = CX_CREATE(cxFollow);
    cxFollowInit(this, pview, speed, target);
    return this;
}

