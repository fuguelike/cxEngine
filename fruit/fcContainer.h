//
//  fcContainer.h
//  cxEngine
//  容器基类
//  Created by xuhua on 3/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcTreasureBox_h
#define fruit_fcTreasureBox_h

#include "fcSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcContainer, fcSprite)
    fcContainerType type;
CX_OBJECT_END(fcContainer)

void fcContainerInit(cxAny this,cxAny map,fcContainerType type);

CX_C_END

#endif
