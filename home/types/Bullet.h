//
//  Bullet.h
//  Home
//
//  Created by xuhua on 9/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Bullet_h
#define Home_Bullet_h

#include <views/cxSprite.h>

CX_C_BEGIN

CX_OBJECT_DEF(Bullet, cxSprite)
    cxAny map;
    cxFloat power;  //子弹携带的攻击力
CX_OBJECT_END(Bullet, cxSprite)

void BulletSetPower(cxAny pview,cxFloat power);

void BulletInit(cxAny pview,cxAny map,cxSize2f size,cxVec2f pos);

cxAny BulletMap(cxAny pview);

CX_C_END

#endif
