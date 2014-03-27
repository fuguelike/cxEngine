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
    CX_METHOD_FIRE(0, sprite->Attacked, sprite, fruit, attacker);
    //移除水果导弹
    fcFruitRemoved(fruit);
}

static cxInt fcFollowFire(cxAny attacker,cxAny fruit,cxAny target)
{
    fcFollow follow = fruit;
    cxViewSetPos(follow, cxViewPosition(attacker));
    cxViewAppend(follow->super.map, follow);
    cxFloat speed = fcMapScaleValue(follow->super.map, follow->super.speed);
    cxFollow followAction = cxFollowCreate(speed, target);
    CX_EVENT_APPEND(followAction->super.onStop, fcFollowStop, cxEventArgWeakRef(attacker));
    cxViewAppendAction(follow, followAction);
    return 0;
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

fcFollow fcFollowCreate(cxAny map,cxFloat speed)
{
    fcFollow this = CX_CREATE(fcFollow);
    fcFruitInit(this, map, "item.xml?fire.png",speed);
    return this;
}