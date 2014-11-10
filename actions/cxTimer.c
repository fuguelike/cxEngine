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
    CX_RETURN(!this->IsBegin);
    CX_SUPER(cxAction, this, Init, CX_M(void));
    CX_EVENT_FIRE(this, onArrive);
}
CX_METHOD_DEF(cxTimer,Exit,cxBool)
{
    this->Repeat --;
    CX_RETURN(this->Repeat == 0 && this->IsBegin, true);
    CX_EVENT_FIRE(this, onArrive);
    return this->Repeat == 0;
}
CX_SETTER_DEF(cxTimer, repeat)
{
    this->IsBegin = cxJsonToInt(value, this->Repeat);
}
CX_SETTER_DEF(cxTimer, begin)
{
    this->IsBegin = cxJsonToBool(value, this->IsBegin);
}
CX_TYPE(cxTimer, cxAction)
{
    CX_SETTER(cxTimer, repeat);
    CX_SETTER(cxTimer, begin);
    
    CX_METHOD(cxTimer, Init);
    CX_METHOD(cxTimer, Exit);
}
CX_INIT(cxTimer, cxAction)
{
    this->IsBegin = false;
}
CX_FREE(cxTimer, cxAction)
{
    CX_EVENT_RELEASE(this->onArrive);
}
CX_TERM(cxTimer, cxAction)

cxTimer cxTimerCreate(cxFloat freq,cxInt repeat)
{
    cxTimer this = CX_CREATE(cxTimer);
    this->cxAction.Time  = freq;
    cxTimerSetRepeat(this, repeat);
    return this;
}

