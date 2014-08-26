//
//  cxSpline.c
//  cxEngine
//
//  Created by xuhua on 12/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxSpline.h"

static void cxSplineInit(cxAny pav)
{
    cxSpline this = pav;
    CX_ASSERT(this->cxAction.view != NULL, "view not set");
    this->delta = 1.0f/((cxFloat)cxAnyArrayLength(this->points) - 1.0f);
    this->diff = cxVec2fv(0, 0);
    this->prev = this->cxAction.view->position;
}

static cxVec2f cxSplinePointAt(cxSpline this,cxInt idx)
{
    idx = CX_MIN(cxAnyArrayLength(this->points) - 1, CX_MAX(idx, 0));
    return *cxAnyArrayAt(this->points, idx, cxVec2f);
}

static void cxSplineStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxInt index = 0;
    cxSpline this = pav;
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
    cxVec2f p0 = cxSplinePointAt(this, this->index - 1);
    cxVec2f p1 = cxSplinePointAt(this, this->index + 0);
    cxVec2f p2 = cxSplinePointAt(this, this->index + 1);
    cxVec2f p3 = cxSplinePointAt(this, this->index + 2);
    cxVec2f newpos = cxCardinalSplineAt(p0, p1, p2, p3, this->tension, lt);
    //
    cxVec2f diff;
    kmVec2Subtract(&diff, &this->cxAction.view->position, &this->prev);
    if(diff.x != 0 || diff.y != 0){
        kmVec2Add(&this->diff, &this->diff, &diff);
        kmVec2Add(&newpos, &newpos, &this->diff);
    }
    //
    cxViewSetPos(this->cxAction.view, newpos);
    this->prev = newpos;
}

static void cxSplineReset(cxAny pav)
{
    cxSpline this = pav;
    this->index = -1;
    cxAnyArrayClean(this->points);
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
    CX_METHOD_SET(this->cxAction.Init, cxSplineInit);
    CX_METHOD_SET(this->cxAction.Step, cxSplineStep);
    CX_METHOD_SET(this->cxAction.Reset, cxSplineReset);
    this->points = cxAnyArrayAlloc(cxVec2f);
}
CX_OBJECT_FREE(cxSpline, cxAction)
{
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

void cxSplineAppend(cxAny pav,cxVec2f pos)
{
    cxSpline this = pav;
    cxAnyArrayAppend(this->points, &pos);
}


