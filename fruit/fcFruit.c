//
//  fcFruit.c
//  fruit
//
//  Created by xuhua on 3/26/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcFruit.h"
#include "fcSprite.h"
#include "fcMap.h"

void fcFruitRemoved(cxAny this)
{
    fcFruit s = this;
    CX_ASSERT(s->map != NULL, "not join map");
    cxViewRemoved(s);
}

CX_OBJECT_INIT(fcFruit, cxSprite)
{
    
}
CX_OBJECT_FREE(fcFruit, cxSprite)
{
    CX_METHOD_RELEASE(this->Fire);
}
CX_OBJECT_TERM(fcFruit, cxSprite)

void fcFruitInit(cxAny this,cxAny map,cxConstChars url,cxFloat speed)
{
    fcFruit fruit = this;
    fruit->map = map;
    fruit->speed = speed;
    cxSpriteSetTextureURL(fruit, url, true);
}

cxInt fcFruitFire(cxAny this,cxAny sprite,cxAny target)
{
    fcFruit fruit = this;
    //盯住目标
    fcSpriteLookAt(sprite, target);
    //发射
    return CX_METHOD_FIRE(0, fruit->Fire, this, sprite, target);
}