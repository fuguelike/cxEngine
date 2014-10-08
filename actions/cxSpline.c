//
//  cxSpline.c
//  cxEngine
//
//  Created by xuhua on 12/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxSpline.h"

cxVec2f cxSplinePointAt(cxAny pav,cxInt idx)
{
    CX_ASSERT_THIS(pav, cxSpline);
    idx = CX_MIN(cxAnyArrayLength(this->points) - 1, CX_MAX(idx, 0));
    return *cxAnyArrayAt(this->points, idx, cxVec2f);
}

static void cxSplineInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxSpline);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxInt num = cxAnyArrayLength(this->points);
    if(num < 2){
        cxActionStop(pav);
        return;
    }
    this->delta = 1.0f/((cxFloat)num - 1.0f);
    this->diff = cxVec2fv(0, 0);
    this->prev =cxViewGetPosition(view);
    
    cxVec2f p0 = cxSplinePointAt(this, 0);
    cxVec2f p1 = cxSplinePointAt(this, num - 1);
    cxFloat angle = cxVec2fRadiansBetween(p1, p0);
    if(!cxFloatEqu(angle, this->angle)){
        this->angle = angle;
        CX_EVENT_FIRE(this, onAngle);
    }
}

static void cxSplineStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxInt index = 0;
    CX_ASSERT_THIS(pav, cxSpline);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, view);
    cxFloat lt = 0;
    if (time >= 1.0f){
        index = cxAnyArrayLength(this->points) - 1;
        lt = 1.0f;
    }else{
        index = time / this->delta;
        lt = (time - this->delta * (cxFloat)index) / this->delta;
    }
    if(index != this->index){
        this->index = index;
        CX_EVENT_FIRE(this, onIndex);
    }
    cxVec2f p0 = cxSplinePointAt(this, index - 1);
    cxVec2f p1 = cxSplinePointAt(this, index + 0);
    cxVec2f p2 = cxSplinePointAt(this, index + 1);
    cxVec2f p3 = cxSplinePointAt(this, index + 2);
    cxVec2f newpos = cxCardinalSplineAt(p0, p1, p2, p3, this->tension, lt);
    cxVec2f vpos = cxViewGetPosition(view);
    cxVec2f diff = cxVec2fSub(vpos, this->prev);
    if(diff.x != 0 || diff.y != 0){
        this->diff = cxVec2fAdd(this->diff, diff);
        newpos = cxVec2fAdd(newpos, this->diff);
    }
    cxFloat angle = cxVec2fRadiansBetween(newpos, this->prev);
    if(!cxFloatEqu(angle, this->angle)){
        this->angle = angle;
        CX_EVENT_FIRE(this, onAngle);
    }
    cxViewSetPosition(view, newpos);
    this->prev = newpos;
}

static void cxSplineReset(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxSpline);
    this->index = -1;
    this->angle = INT32_MAX;
    cxAnyArrayClear(this->points);
}

CX_SETTER_DEF(cxSpline, points)
{
    cxJson points = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(points, item)
    {
        cxVec2f point = cxJsonToVec2f(item, cxVec2fv(0, 0));
        cxSplineAppend(this, point);
    }
    CX_JSON_ARRAY_EACH_END(points, item)
}

CX_OBJECT_TYPE(cxSpline, cxAction)
{
    CX_PROPERTY_SETTER(cxSpline, points);
}
CX_OBJECT_INIT(cxSpline, cxAction)
{
    this->index = -1;
    this->angle = INT32_MAX;
    CX_SET(cxAction, this, Init, cxSplineInit);
    CX_SET(cxAction, this, Step, cxSplineStep);
    CX_SET(cxAction, this, Reset, cxSplineReset);
    this->points = cxAnyArrayAlloc(cxVec2f);
}
CX_OBJECT_FREE(cxSpline, cxAction)
{
    CX_EVENT_RELEASE(this->onAngle);
    CX_EVENT_RELEASE(this->onIndex);
    CX_RELEASE(this->points);
}
CX_OBJECT_TERM(cxSpline, cxAction)

cxSpline cxSplineCreate(cxFloat time,cxAnyArray points)
{
    CX_ASSERT(points != NULL, "points null");
    cxSpline this = CX_CREATE(cxSpline);
    cxActionSetTime(this, time);
    CX_RETAIN_SWAP(this->points, points);
    return this;
}

void cxSplineRemove(cxAny pav,cxInt num)
{
    CX_ASSERT_THIS(pav, cxSpline);
    if(num > cxAnyArrayLength(this->points) || num == 0){
        return;
    }
    for(cxInt i=0; i < num; i++){
        cxAnyArrayPop(this->points);
    }
}

void cxSplineAppend(cxAny pav,cxVec2f pos)
{
    CX_ASSERT_THIS(pav, cxSpline);
    cxAnyArrayAppend(this->points, &pos);
}


