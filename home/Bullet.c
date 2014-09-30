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

static cxAny BulletCreateEngineDefault(cxAny bullet)
{
    cxAny target = NULL;
    BulletGetNode(bullet, NULL, &target);
    CX_RETURN(target == NULL,NULL);
    return cxFollowCreate(500, target);
}


CX_OBJECT_TYPE(Bullet, cxSprite)
{
    
}
CX_OBJECT_INIT(Bullet, cxSprite)
{
    cxSpriteSetTextureURL(this, "bullet.json?shell.png");
    SET(Bullet, this, CreateEngine, BulletCreateEngineDefault);
}
CX_OBJECT_FREE(Bullet, cxSprite)
{
    
}
CX_OBJECT_TERM(Bullet, cxSprite)

cxAny BulletCreateEngine(cxAny pview)
{
    CX_ASSERT_THIS(pview, Bullet);
    cxAny action = CX_METHOD_GET(NULL, this->CreateEngine, this);
    CX_RETURN(action == NULL, NULL);
    BulletSetAction(this, action);
    return action;
}

void BulletGetNode(cxAny pview,cxAny *pattacker,cxAny *ptarget)
{
    CX_ASSERT_THIS(pview, Bullet);
    cxHash bindes = cxViewGetBindes(this);
    CX_HASH_FOREACH(bindes, ele, tmp){
        cxInt type = cxNumberToInt(ele->any);
        if(type == BulletBindReasonAttacker && pattacker != NULL){
            *pattacker = cxHashKeyToAny(ele);
        }
        if(type == BulletBindReasonTarget && ptarget != NULL){
            *ptarget = cxHashKeyToAny(ele);
        }
    }
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
    cxViewSetPosition(this, pos);
}