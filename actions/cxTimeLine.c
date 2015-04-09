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

CX_METHOD_DEF(cxTimeLine,Init,void)
{
    this->Index = -1;
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxTimeLine,Step,void,cxFloat dt,cxFloat time)
{
    cxInt count = cxAnyArrayLength(this->times);
    cxFloat elapsed = cxActionGetTimeElapsed(this);
    for(cxInt i = this->Index + 1; i < count; i++){
        cxFloat time = cxTimeLineGetTime(this, i);
        if(time > elapsed){
            break;
        }
        this->Index = i;
        CX_CALL(this, OnTime, CX_M(void));
    }
}
CX_SETTER_DEF(cxTimeLine, times)
{
    cxJson times = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(times, item)
    cxFloat time = cxJsonToDouble(item, -1);
    if(time < 0){
        continue;
    }
    cxTimeLineSet(this, time);
    CX_JSON_ARRAY_EACH_END(times, item)
}
CX_METHOD_DEF(cxTimeLine, OnTime, void)
{
    
}
CX_TYPE(cxTimeLine, cxAction)
{
    CX_SETTER(cxTimeLine, times);
    
    CX_METHOD(cxTimeLine, OnTime);
    CX_METHOD(cxTimeLine, Init);
    CX_METHOD(cxTimeLine, Step);
}
CX_INIT(cxTimeLine, cxAction)
{
    this->times = cxAnyArrayAlloc(cxFloat);
    cxActionSetTime(this, CX_FOREVER);
}
CX_FREE(cxTimeLine, cxAction)
{
    CX_RELEASE(this->times);
}
CX_TERM(cxTimeLine, cxAction)

cxFloat cxTimeLineGetTime(cxAny pav,cxInt idx)
{
    CX_ASSERT_THIS(pav, cxTimeLine);
    CX_ASSERT(idx >= 0 && idx < cxAnyArrayLength(this->times),"idx out");
    return *cxAnyArrayAt(this->times, idx, cxFloat);
}

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








