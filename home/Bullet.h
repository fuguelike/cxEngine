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
    CX_FIELD_DEF(cxAny Map);
    CX_FIELD_DEF(cxFloat Power);//子弹携带的攻击力
CX_OBJECT_END(Bullet, cxSprite)

CX_FIELD_IMP(Bullet, cxFloat, Power);
CX_FIELD_GET(Bullet, cxAny, Map);

void BulletInit(cxAny pview,cxAny pmap,cxSize2f size,cxVec2f pos);

CX_C_END

#endif
