//
//  fcTreasureBox.c
//  fruit
//
//  Created by xuhua on 3/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcTreasureBox.h"

CX_OBJECT_INIT(fcTreasureBox, fcSprite)
{
    this->super.type = fcSpriteTypeContainer;
}
CX_OBJECT_FREE(fcTreasureBox, fcSprite)
{
    
}
CX_OBJECT_TERM(fcTreasureBox, fcSprite)

void fcTreasureBoxInit(fcTreasureBox this,cxAny map)
{
    fcSpriteInit(this, map);
}

