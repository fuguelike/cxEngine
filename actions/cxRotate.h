//
//  cxRotate.h
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxRotate_h
#define cxEngine_cxRotate_h

#include <core/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxRotate, cxAction)
    cxVec3f raxis;
    cxFloat newAngle;
    cxFloat delta;
    cxFloat oldAngle;
CX_OBJECT_END(cxRotate)

cxRotate cxRotateCreate(cxFloat duration,cxVec3f raxis,cxFloat newAngle);

CX_C_END

#endif
