//
//  Bullet.c
//  Home
//
//  Created by xuhua on 9/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxFollow.h>
#include "Bullet.h"
#include "Define.h"
#include "Node.h"

static cxAny BulletCreateEngine(cxAny bullet)
{
    cxAny target = BulletGetTarget(bullet);
    if(target == NULL){
        return NULL;
    }
    return cxFollowCreate(500, target);
}


CX_OBJECT_TYPE(Bullet, cxSprite)
{
    
}
CX_OBJECT_INIT(Bullet, cxSprite)
{
    cxSpriteSetTextureURL(this, "bullet.json?shell.png");
    SET(Bullet, this, CreateEngine, BulletCreateEngine);
}
CX_OBJECT_FREE(Bullet, cxSprite)
{
    
}
CX_OBJECT_TERM(Bullet, cxSprite)

cxAny BulletGetAttacker(cxAny pview)
{
    CX_ASSERT_THIS(pview, Bullet);
    cxHash bindes = cxViewBindes(this);
    CX_HASH_FOREACH(bindes, ele, tmp){
        if(cxNumberToInt(ele->any) == BulletBindReasonAttacker){
            return cxHashKeyToAny(ele);
        }
    }
    return NULL;
}

cxAny BulletGetTarget(cxAny pview)
{
    CX_ASSERT_THIS(pview, Bullet);
    cxHash bindes = cxViewBindes(this);
    CX_HASH_FOREACH(bindes, ele, tmp){
        if(cxNumberToInt(ele->any) == BulletBindReasonTarget){
            return cxHashKeyToAny(ele);
        }
    }
    return NULL;
}

void BulletBind(cxAny pview,cxAny pattacker,cxAny ptarget)
{
    CX_ASSERT_THIS(pview, Bullet);
    CX_ASSERT_VALUE(pattacker, Node, attacker);
    CX_ASSERT_VALUE(ptarget, Node, target);
    cxViewBind(this, target, cxNumberInt(BulletBindReasonTarget));
    cxViewBind(this, attacker, cxNumberInt(BulletBindReasonAttacker));
}


void BulletInit(cxAny pview,cxAny pmap,cxSize2f size,cxVec2f pos)
{
    CX_ASSERT_THIS(pview, Bullet);
    this->Map = pmap;
    cxViewSetSize(this, size);
    cxViewSetPos(this, pos);
}