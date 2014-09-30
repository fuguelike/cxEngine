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
    CX_FIELD_DEF(cxAny Action); //子弹驱动action
    CX_METHOD_DEF(void, onUpdate,cxAny bullet,cxAny action);//驱动器update
    CX_METHOD_DEF(cxAny, CreateEngine,cxAny bullet);//创建一个驱动bullet的action发动机
CX_OBJECT_END(Bullet, cxSprite)

CX_FIELD_IMP(Bullet, cxFloat, Power);
CX_FIELD_GET(Bullet, cxAny, Map);
CX_FIELD_IMP(Bullet, cxAny, Action);

void BulletBind(cxAny pview,cxAny pattacker,cxAny target);

void BulletGetNode(cxAny pview,cxAny *pattacker,cxAny *ptarget);

void BulletInit(cxAny pview,cxAny pmap,cxSize2f size,cxVec2f pos);

CX_C_END

#endif
