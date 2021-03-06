//
//  cxScale.h
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxScale_h
#define cxEngine_cxScale_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_DEF(cxScale, cxAction)
    cxVec2f oldScale;
    cxVec2f delta;
    cxVec2f newScale;
CX_END(cxScale, cxAction)

cxScale cxScaleCreate(cxFloat time,cxVec2f scale);

void cxScaleSetScale(cxAny pav,cxVec2f scale);

CX_C_END

#endif
