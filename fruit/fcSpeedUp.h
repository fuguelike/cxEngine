//
//  fcSpeedUp.h
//  cxEngine
//  加速道具
//  Created by xuhua on 3/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcSpeedUp_h
#define fruit_fcSpeedUp_h

#include "fcSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcSpeedUp, fcSprite)
    cxFloat speed;//增加的速度
    cxFloat time;//持续的时间
CX_OBJECT_END(fcSpeedUp)

void fcSpeedUpInit(cxAny this,cxAny map);

CX_C_END

#endif
