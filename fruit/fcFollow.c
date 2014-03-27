//
//  fcFollow.c
//  fruit
//
//  Created by xuhua on 3/26/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxFollow.h>
#include "fcMap.h"
#include "fcSprite.h"
#include "fcFollow.h"

static void fcFollowStop(cxEvent *e)
{
    cxAny attacker = cxEventArgToWeakRef(e->args);
    fcFollow fruit = cxActionView(e->sender);
    fcSprite sprite = cxFollowTarget(e->sender);
    //目标被攻击了
    CX_METHOD_RUN(sprite->Attacked, sprite, fruit, attacker);
    //移除水果导弹
    fcFruitRemoved(fruit);
}

static void fcFollowFire(cxAny fruit, cxAny attacker,cxAny target)
{
    fcFollow this = fruit;
    cxViewSetPos(this, cxViewPosition(attacker));
    cxViewAppend(this->super.map, this);
    cxFloat speed = fcMapScaleValue(this->super.map, this->super.speed);
    cxFollow follow = cxFollowCreate(speed, target);
    CX_EVENT_APPEND(follow->super.onStop, fcFollowStop, cxEventArgWeakRef(attacker));
    cxViewAppendAction(this, follow);
}

CX_OBJECT_INIT(fcFollow, fcFruit)
{
    this->super.attackPower = 10;
    this->super.type = fcFruitTypeFollow;
    CX_METHOD_OVERRIDE(this->super.Fire, fcFollowFire);
}
CX_OBJECT_FREE(fcFollow, fcFruit)
{
    
}
CX_OBJECT_TERM(fcFollow, fcFruit)

void fcFollowInit(fcFollow this,cxAny map,cxFloat speed)
{
    fcFruitInit(this, map,speed);
}