//
//  cxSpline.h
//  cxEngine
//
//  Created by xuhua on 12/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxSpline_h
#define cxEngine_cxSpline_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxSpline, cxAction)
    cxVec2f diff;
    cxVec2f prev;
    cxAnyArray points;
    cxFloat tension;
    cxFloat delta;
    cxInt index;
    CX_EVENT_ALLOC(onIndex);
CX_OBJECT_END(cxSpline, cxAction)

void cxSplineAppend(cxAny pav,cxVec2f pos);

cxSpline cxSplineCreate(cxFloat duration, cxAnyArray points);

CX_C_END

#endif
