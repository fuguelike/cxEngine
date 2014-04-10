//
//  fcTreasureBox.c
//  fruit
//
//  Created by xuhua on 3/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcContainer.h"

CX_OBJECT_INIT(fcContainer, fcSprite)
{
    this->super.type = fcSpriteTypeContainer;
}
CX_OBJECT_FREE(fcContainer, fcSprite)
{
    
}
CX_OBJECT_TERM(fcContainer, fcSprite)

void fcContainerInit(cxAny this,cxAny map,fcContainerType type)
{
    fcContainer c = this;
    c->type = type;
    fcSpriteInit(this, map);
}

