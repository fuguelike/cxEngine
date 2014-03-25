//
//  fcAttacker.h
//  cxEngine
//
//  Created by xuhua on 3/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcAttacker_h
#define fruit_fcAttacker_h

#include "fcSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcAttacker, fcSprite)
    fcAttackerType attackerType;
CX_OBJECT_END(fcAttacker)

fcAttacker fcAttackerCreate(cxAny map,cxVec2i idx,fcAttackerType type);

CX_C_END

#endif
