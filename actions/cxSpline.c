//
//  cxSpline.c
//  cxEngine
//
//  Created by xuhua on 12/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxNumber.h>
#include <core/cxActionRoot.h>
#include "cxSpline.h"

static void cxSplineInit(cxAny pav)
{
    cxSpline this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    this->delta = 1.0f/((cxFloat)cxArrayLength(this->points) - 1.0f);
}

static cxVec2f cxSplinePointAt(cxSpline this,cxInt idx)
{
    idx = CX_MIN(cxArrayLength(this->points) - 1, CX_MAX(idx, 0));
    cxNumber num = cxArrayAtIndex(this->points, idx);
    CX_ASSERT(num != NULL, "num error");
    return cxNumberToVec2f(num);
}

static void cxSplineStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxInt index = 0;
    cxSpline this = pav;
    cxFloat lt = 0;
    if (time >= 1.0f){
        index = cxArrayLength(this->points) - 1;
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
    cxViewSetPos(this->super.view, newpos);
}

static void cxSplineReadAttr(cxReaderAttrInfo *info)
{
    cxActionReadAttr(info);
    cxSpline this = info->object;    
    this->tension = cxXMLReadFloatAttr(info, "cxSpline.tension", this->tension);
    cxTypes types = cxXMLReadTypesAttr(info, "cxSpline.points");
    if(cxTypesIsType(types, cxTypesArray)){
        cxArrayAppends(this->points, types->any);
    }
    cxXMLAppendEvent(info, this, cxSpline, onIndex);
}

static void cxSplineReset(cxAny pav)
{
    cxSpline this = pav;
    this->index = -1;
    cxArrayClean(this->points);
}

CX_OBJECT_INIT(cxSpline, cxAction)
{
    this->index = -1;
    cxObjectSetReadAttrFunc(this, cxSplineReadAttr);
    CX_METHOD_OVERRIDE(this->super.Init, cxSplineInit);
    CX_METHOD_OVERRIDE(this->super.Step, cxSplineStep);
    CX_METHOD_OVERRIDE(this->super.Reset, cxSplineReset);
    this->points = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxSpline, cxAction)
{
    CX_EVENT_RELEASE(this->onIndex);
    CX_RELEASE(this->points);
}
CX_OBJECT_TERM(cxSpline, cxAction)

void cxSplineAppend(cxAny pav,cxVec2f pos)
{
    cxSpline this = pav;
    cxArrayAppend(this->points, cxNumberVec2f(pos));
}