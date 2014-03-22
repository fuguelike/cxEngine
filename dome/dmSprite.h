//
//  dmSprite.h
//  cxEngine
//
//  Created by xuhua on 3/21/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef dome_dmSprite_h
#define dome_dmSprite_h

#include <views/cxSprite.h>

CX_C_BEGIN

CX_OBJECT_DEF(dmSprite, cxSprite)
    cxAny map;
CX_OBJECT_END(dmSprite)

dmSprite dmSpriteCreate(cxAny map,cxVec2i idx);

CX_C_END

#endif
