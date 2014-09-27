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


void BulletInit(cxAny pview,cxAny pmap,cxSize2f size,cxVec2f pos)
{
    CX_ASSERT_THIS(pview, Bullet);
    this->Map = pmap;
    cxViewSetSize(this, size);
    cxViewSetPos(this, pos);
}