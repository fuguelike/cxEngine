//
//  cxTimer.c
//  cxEngine
//
//  Created by xuhua on 10/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTimer.h"

CX_METHOD_DEF(cxTimer,Init,void)
{
    this->time = 0;
    cxTimerSetRepeat(this, this->repeat);
    cxActionSetTime(this, this->freq * this->repeat);
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxTimer,Step,void,cxFloat dt,cxFloat time)
{
    this->time += dt;
    if(this->time >= this->freq){
        this->Repeat --;
        CX_CALL(this, OnArrive, CX_M(void));
        this->time -= this->freq;
    }
    if(this->Repeat == 0){
        cxActionStop(this);
    }
}
CX_SETTER_DEF(cxTimer, repeat)
{
    this->Repeat = cxJsonToInt(value, this->Repeat);
    this->repeat = this->Repeat;
}
CX_SETTER_DEF(cxTimer, freq)
{
    this->freq = cxJsonToDouble(value, 0.0f);
}
CX_METHOD_DEF(cxTimer, OnArrive, void)
{
    CX_EVENT_FIRE(this, onArrive);
}
CX_TYPE(cxTimer, cxAction)
{
    CX_SETTER(cxTimer, repeat);
    CX_SETTER(cxTimer, freq);
    
    CX_METHOD(cxTimer, OnArrive);
    
    CX_METHOD(cxTimer, Init);
    CX_METHOD(cxTimer, Step);
}
CX_INIT(cxTimer, cxAction)
{
    
}
CX_FREE(cxTimer, cxAction)
{
    CX_EVENT_RELEASE(this->onArrive);
}
CX_TERM(cxTimer, cxAction)

cxTimer cxTimerCreate(cxFloat freq,cxInt repeat)
{
    cxTimer this = CX_CREATE(cxTimer);
    cxTimerSetRepeat(this, repeat);
    this->repeat = repeat;
    this->freq = freq;
    return this;
}

