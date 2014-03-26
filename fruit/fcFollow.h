//
//  fcFollow.h
//  cxEngine
//
//  Created by xuhua on 3/26/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcFollow_h
#define fruit_fcFollow_h

#include "fcFruit.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcFollow, fcFruit)
    
CX_OBJECT_END(fcFollow)

fcFollow fcFollowCreate(cxAny map,cxFloat speed);

CX_C_END

#endif
