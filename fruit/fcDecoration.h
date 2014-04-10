//
//  fcDecoration.h
//  cxEngine
//  装饰品基类
//  Created by xuhua on 4/10/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcDecoration_h
#define fruit_fcDecoration_h

#include "fcSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcDecoration, fcSprite)
    fcDecorationType type;
CX_OBJECT_END(fcDecoration)

void fcDecorationInit(cxAny pview,cxAny map,fcDecorationType type);

CX_C_END

#endif
