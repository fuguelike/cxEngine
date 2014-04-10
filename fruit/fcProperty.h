//
//  fcSpeedUp.h
//  cxEngine
//  道具基类
//  Created by xuhua on 3/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcSpeedUp_h
#define fruit_fcSpeedUp_h

#include "fcSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcProperty, fcSprite)
    fcPropertyType type;
CX_OBJECT_END(fcProperty)

void fcPropertyInit(cxAny this,cxAny map,fcPropertyType type);

CX_C_END

#endif
