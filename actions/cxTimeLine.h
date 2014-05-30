//
//  cxTimeLine.h
//  cxEngine
//
//  Created by xuhua on 10/29/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTimeLine_h
#define cxEngine_cxTimeLine_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxTimeLine, cxAction)
    cxInt index;
    cxArray times;
    CX_EVENT_ALLOC(onTime);
CX_OBJECT_END(cxTimeLine, cxAction)

//set time event point
void cxTimeLineSet(cxAny pav,cxFloat time);

//add time ,lasttime + add
void cxTimeLineAdd(cxAny pav,cxFloat add);

CX_C_END

#endif
