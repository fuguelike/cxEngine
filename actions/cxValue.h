//
//  cxValue.h
//  cxEngine
//  cxFlaot Value action
//  Created by xuhua on 3/5/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngine_cxValue_h
#define cxEngine_cxValue_h

#include <engine/cxAction.h>

CX_C_BEGIN

#define CX_VALUE_MAX        4

CX_DEF(cxValue, cxAction)
    cxFloat *ptr[CX_VALUE_MAX];     //must from action view
    cxFloat delta[CX_VALUE_MAX];
    cxFloat from[CX_VALUE_MAX];
    cxFloat to[CX_VALUE_MAX];
    cxInt count;                   //value count
CX_END(cxValue, cxAction)

cxValue cxValueCreateFloat(cxFloat time,cxFloat *ptr,cxFloat to);

cxValue cxValueCreateVec2f(cxFloat time,cxVec2f *ptr,cxVec2f to);

CX_C_END

#endif
