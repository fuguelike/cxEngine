//
//  cxParabola.h
//  cxEngine
//   抛物线
//  Created by xuhua on 1/9/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxParabola_h
#define cxEngine_cxParabola_h

#include <core/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxParabola, cxAction)
    cxVec2f pos;
    cxVec2f gravity;
    cxVec2f speed;
    cxFloat angle;
    cxFloat time;
CX_OBJECT_END(cxParabola)

cxParabola cxParabolaCreate(cxVec2f speed);

CX_C_END

#endif
