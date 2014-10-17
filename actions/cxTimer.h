//
//  cxTimer.h
//  cxEngine
//
//  Created by xuhua on 10/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTimer_h
#define cxEngine_cxTimer_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_DEF(cxTimer, cxAction)
    CX_FIELD_DEF(cxInt Repeat);
    CX_FIELD_DEF(cxBool IsBegin);
    CX_EVENT_ALLOC(onArrive);
CX_END(cxTimer, cxAction)

CX_FIELD_IMP(cxTimer, cxInt, Repeat);
CX_FIELD_IMP(cxTimer, cxBool, IsBegin);

cxTimer cxTimerCreate(cxFloat freq,cxInt repeat);

CX_C_END

#endif
