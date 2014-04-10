//
//  fcSpeedUp.c
//  fruit
//  加速道具
//  Created by xuhua on 3/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcMap.h"
#include "fcIntruder.h"
#include "fcProperty.h"

//props 被 target 碰到
static void fcPropertyCollide(cxAny props,cxAny sprite)
{
    fcProperty this = props;
    CX_LOGGER("fcProperty Collide target");
    fcMapRemoveProps(this->super.map, props);
}

CX_OBJECT_INIT(fcProperty, fcSprite)
{
    this->super.type = fcSpriteTypeProperty;
    CX_METHOD_OVERRIDE(this->super.Collide, fcPropertyCollide);
}
CX_OBJECT_FREE(fcProperty, fcSprite)
{

}
CX_OBJECT_TERM(fcProperty, fcSprite)

void fcPropertyInit(cxAny this,cxAny map,fcPropertyType type)
{
    fcProperty p = this;
    p->type = type;
    fcSpriteInit(this, map);
}