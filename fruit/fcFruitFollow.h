//
//  fcFruitFollow.h
//  cxEngine
//
//  Created by xuhua on 3/26/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcFruitFollow_h
#define fruit_fcFruitFollow_h

#include "fcFruit.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcFruitFollow, fcFruit)
    
CX_OBJECT_END(fcFruitFollow)

fcFruitFollow fcFruitFollowCreate(cxAny map,cxFloat speed);

CX_C_END

#endif
