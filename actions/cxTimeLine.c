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
    cxFloat *time = cxAnyArrayLast(this->times, cxFloat);
    if(time == NULL){
        cxActionStop(this);
        return;
    }
    cxActionSetTime(this, *time + 0.5f);
    this->index = -1;
}

static void cxTimeLineStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxTimeLine);
    cxInt count = cxAnyArrayLength(this->times);
    cxFloat elapsed = cxActionGetTimeElapsed(this);
    for(cxInt i = this->index + 1; i < count; i++){
        cxFloat *time = cxAnyArrayAt(this->times, i, cxFloat);
        if(*time < elapsed){
            break;
        }
        this->index = i;
        CX_EVENT_FIRE(this, onTime);
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
        if(time < 0){
            continue;
        }
        cxTimeLineSet(this, time);
    }
    CX_JSON_ARRAY_EACH_END(times, item)
}

CX_TYPE(cxTimeLine, cxAction)
{
    CX_PROPERTY_SETTER(cxTimeLine, times);
}
CX_INIT(cxTimeLine, cxAction)
{
    CX_SET(cxAction, this, Init, cxTimeLineInit);
    CX_SET(cxAction, this, Step, cxTimeLineStep);
    this->times = cxAnyArrayAlloc(cxFloat);
}
CX_FREE(cxTimeLine, cxAction)
{
    CX_EVENT_RELEASE(this->onTime);
    CX_RELEASE(this->times);
}
CX_TERM(cxTimeLine, cxAction)

void cxTimeLineSet(cxAny pav,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxTimeLine);
    cxAnyArrayAppend(this->times, &time);
}

void cxTimeLineAdd(cxAny pav,cxFloat add)
{
    CX_ASSERT_THIS(pav, cxTimeLine);
    cxFloat time = add;
    cxFloat *last = cxAnyArrayLast(this->times,cxFloat);
    if(last != NULL){
        time += *last;
    }
    cxTimeLineSet(pav, time);
}








