//
//  Bullet.c
//  Home
//
//  Created by xuhua on 9/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "Bullet.h"

CX_OBJECT_TYPE(Bullet, cxSprite)
{
    
}
CX_OBJECT_INIT(Bullet, cxSprite)
{
    cxSpriteSetTextureURL(this, "bullet.json?shell.png");
}
CX_OBJECT_FREE(Bullet, cxSprite)
{
    
}
CX_OBJECT_TERM(Bullet, cxSprite)

void BulletSetPower(cxAny pview,cxFloat power)
{
    CX_ASSERT_THIS(pview, Bullet);
    this->power = power;
}

cxAny BulletMap(cxAny pview)
{
    CX_ASSERT_THIS(pview, Bullet);
    return this->map;
}

void BulletInit(cxAny pview,cxAny map,cxSize2f size,cxVec2f pos)
{
    CX_ASSERT_THIS(pview, Bullet);
    this->map = map;
    cxViewSetSize(this, size);
    cxViewSetPos(this, pos);
}