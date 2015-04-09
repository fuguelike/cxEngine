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

CX_METHOD_DEF(cxSpline,Init,void)
{
    cxInt num = cxAnyArrayLength(this->points);
    if(num < 2){
        cxActionStop(this);
        return;
    }
    this->delta = 1.0f/((cxFloat)num - 1.0f);
    cxVec2f p0 = cxSplinePointAt(this, 0);
    cxVec2f p1 = cxSplinePointAt(this, num - 1);
    cxFloat angle = cxVec2fRadiansBetween(p1, p0);
    if(!cxFloatEqu(angle, this->angle)){
        this->angle = angle;
        CX_CALL(this, OnAngle, CX_M(void,cxFloat),angle);
    }
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxSpline,Step,void,cxFloat dt,cxFloat time)
{
    cxInt index = 0;
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
        CX_CALL(this, OnIndex, CX_M(void,cxInt),index);
    }
    cxVec2f p0 = cxSplinePointAt(this, index - 1);
    cxVec2f p1 = cxSplinePointAt(this, index + 0);
    cxVec2f p2 = cxSplinePointAt(this, index + 1);
    cxVec2f p3 = cxSplinePointAt(this, index + 2);
    
    cxVec2f newpos = cxCardinalSplineAt(p0, p1, p2, p3, this->tension, lt);
    cxVec2f curr = cxViewGetPosition(view);
    
    cxFloat angle = cxVec2fRadiansBetween(newpos, curr);
    if(!cxFloatEqu(angle, this->angle)){
        this->angle = angle;
        CX_CALL(this, OnAngle, CX_M(void,cxFloat),angle);
    }
    
    cxViewSetPosition(view, newpos);
}
CX_METHOD_DEF(cxSpline,Reset,void)
{
    this->index = -1;
    this->angle = INT32_MAX;
    cxAnyArrayClear(this->points);
    CX_SUPER(cxAction, this, Reset, CX_M(void));
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
CX_METHOD_DEF(cxSpline, OnAngle, void,cxFloat angle)
{
    
}
CX_METHOD_DEF(cxSpline, OnIndex, void,cxInt index)
{
    
}
CX_TYPE(cxSpline, cxAction)
{
    CX_SETTER(cxSpline, points);
    
    CX_METHOD(cxSpline, OnAngle);
    CX_METHOD(cxSpline, OnIndex);
    CX_METHOD(cxSpline, Reset);
    CX_METHOD(cxSpline, Init);
    CX_METHOD(cxSpline, Step);
}
CX_INIT(cxSpline, cxAction)
{
    this->index = -1;
    this->angle = INT32_MAX;
    this->points = cxAnyArrayAlloc(cxVec2f);
}
CX_FREE(cxSpline, cxAction)
{
    CX_RELEASE(this->points);
}
CX_TERM(cxSpline, cxAction)

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


