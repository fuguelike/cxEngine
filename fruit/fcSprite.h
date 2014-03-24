//
//  fcSprite.h
//  cxEngine
//
//  Created by xuhua on 3/21/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcSprite_h
#define fruit_fcSprite_h

#include <views/cxSprite.h>
#include "fcDefine.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcSprite, cxSprite)
    cxAny map;
    cxVec2i idx;
CX_OBJECT_END(fcSprite)

fcSprite fcSpriteCreate(cxAny map,cxVec2i idx);

CX_C_END

#endif
