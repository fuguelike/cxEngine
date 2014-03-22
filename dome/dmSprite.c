//
//  dmSprite.c
//  dome
//
//  Created by xuhua on 3/21/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "dmMap.h"
#include "dmSprite.h"

CX_OBJECT_INIT(dmSprite, cxSprite)
{
    
}
CX_OBJECT_FREE(dmSprite, cxSprite)
{

}
CX_OBJECT_TERM(dmSprite, cxSprite)

dmSprite dmSpriteCreate(cxAny map,cxVec2i idx)
{
    dmMap dm = map;
    dmSprite this = CX_CREATE(dmSprite);
    cxVec2f pos = dmMapToPos(dm, idx);
    cxViewSetPos(this, pos);
    cxViewSetSize(this, dm->gridSize);
    this->map = map;
    cxSpriteSetTextureURL(this, "item.xml?white.png", false);
    return this;
}