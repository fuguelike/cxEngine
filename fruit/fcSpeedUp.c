//
//  fcSpeedUp.c
//  fruit
//  加速道具
//  Created by xuhua on 3/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcMap.h"
#include "fcIntruder.h"
#include "fcSpeedUp.h"

//props 被 target 碰到
static void fcSpeedUpCollide(cxAny props,cxAny sprite)
{
    CX_LOGGER("fcSpeedUp Collide target");
    fcSpeedUp this = props;
    fcMapRemoveProps(this->super.map, this);
}

CX_OBJECT_INIT(fcSpeedUp, fcSprite)
{
    this->super.type = fcSpriteTypeProperty;
    CX_METHOD_OVERRIDE(this->super.Collide, fcSpeedUpCollide);
}
CX_OBJECT_FREE(fcSpeedUp, fcSprite)
{

}
CX_OBJECT_TERM(fcSpeedUp, fcSprite)

void fcSpeedUpInit(cxAny this,cxAny map)
{
    fcSpriteInit(this, map);
}