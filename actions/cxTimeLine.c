//
//  cxTimeLine.c
//  cxEngine
//
//  Created by xuhua on 10/29/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxTimeLine.h"

// 0.1 0.3 0.6 1.0 2.0

static void cxTimeLineInit(cxAny pav)
{
    cxTimeLine this = pav;
    cxNumber last = cxArrayLast(this->times);
    if(last == NULL){
        cxActionStop(this);
        return;
    }
    this->cxAction.time = cxNumberToFloat(last) + 1.0f;
    this->index = -1;
}

static void cxTimeLineStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxTimeLine this = pav;
    cxInt count = cxArrayLength(this->times);
    for(cxInt i = this->index + 1; i < count; i++){
        cxNumber num = cxArrayAtIndex(this->times, i);
        cxFloat time = cxNumberToFloat(num);
        if(this->cxAction.timeElapsed >= time){
            this->index = i;
            CX_EVENT_FIRE(this, onTime);
            continue;
        }
        break;
    }
    if(this->index == (count - 1)){
        cxActionStop(this);
    }
}

CX_SETTER_DEF(cxTimeLine, times)
{
    cxJson times = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(times, item)
    {
        cxFloat time = cxJsonToDouble(item, -1);
        CX_CONTINUE(time < 0);
        cxTimeLineSet(this, time);
    }
    CX_JSON_ARRAY_EACH_END(times, item)
}

CX_OBJECT_TYPE(cxTimeLine, cxAction)
{
    CX_PROPERTY_SETTER(cxTimeLine, times);
}
CX_OBJECT_INIT(cxTimeLine, cxAction)
{
    CX_METHOD_SET(this->cxAction.Init, cxTimeLineInit);
    CX_METHOD_SET(this->cxAction.Step, cxTimeLineStep);
    this->times = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxTimeLine, cxAction)
{
    CX_EVENT_RELEASE(this->onTime);
    CX_RELEASE(this->times);
}
CX_OBJECT_TERM(cxTimeLine, cxAction)

void cxTimeLineSet(cxAny pav,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxTimeLine);
    cxArrayAppend(this->times, cxNumberFloat(time));
}

void cxTimeLineAdd(cxAny pav,cxFloat add)
{
    CX_ASSERT_THIS(pav, cxTimeLine);
    cxFloat time = add;
    cxNumber last = cxArrayLast(this->times);
    if(last != NULL){
        time += cxNumberToFloat(last);
    }
    cxTimeLineSet(pav, time);
}








