//
//  dmSprite.c
//  dome
//
//  Created by xuhua on 3/21/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcMap.h"
#include "fcSprite.h"

CX_OBJECT_INIT(fcSprite, cxSprite)
{
    
}
CX_OBJECT_FREE(fcSprite, cxSprite)
{

}
CX_OBJECT_TERM(fcSprite, cxSprite)

fcSprite fcSpriteCreate(cxAny map,cxVec2i idx)
{
    fcMap dm = map;
    fcSprite this = CX_CREATE(fcSprite);
    cxVec2f pos = fcMapToPos(dm, idx);
    cxViewSetPos(this, pos);
    cxViewSetSize(this, dm->gridSize);
    this->map = map;
    this->idx = idx;
    return this;
}