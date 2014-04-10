//
//  fcIntruder.h
//  cxEngine
//  进攻者基类
//  Created by xuhua on 3/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcIntruder_h
#define fruit_fcIntruder_h

#include "fcSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcIntruder, fcSprite)
    fcIntruderType type;
    cxFloat rate;//检测频率
CX_OBJECT_END(fcIntruder)

void fcIntruderInit(cxAny this,cxAny map,fcIntruderType type);

void fcIntruderLoop(cxAny this);

CX_C_END

#endif
