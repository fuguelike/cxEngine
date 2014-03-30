//
//  fcIntruder.c
//  fruit
//
//  Created by xuhua on 3/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcIntruder.h"

CX_OBJECT_INIT(fcIntruder, fcSprite)
{
    this->super.type = fcSpriteTypeIntruder;
}
CX_OBJECT_FREE(fcIntruder, fcSprite)
{

}
CX_OBJECT_TERM(fcIntruder, fcSprite)

void fcIntruderInit(cxAny this,cxAny map)
{
    fcSpriteInit(this, map);
}