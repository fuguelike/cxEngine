//
//  fcFollow.c
//  fruit
//
//  Created by xuhua on 3/26/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxFollow.h>
#include <fcMap.h>
#include <fcSprite.h>
#include <fcThrower.h>
#include "fcFollow.h"

cxAny fcFollowMaker(cxAny thrower)
{
    fcThrower this = thrower;
    fcFollow follow = CX_CREATE(fcFollow);
    fcFollowInit(follow, this->super.map, 8);
    cxSpriteSetImage(follow, "item.xml?fire.png");
    return follow;
}

cxBool fcFollowExit(cxAny pav)
{
    cxFollow this = pav;
    cxVec2f tp = cxViewPosition(this->target);
    cxVec2f cp = cxViewPosition(this->super.view);
    cxFloat distance = kmVec2DistanceBetween(&tp, &cp);
    if(distance < 5){
        return true;
    }
    return false;
}

static void fcFollowStop(cxEvent *e)
{
    cxAny attacker = cxEventArgToWeakRef(e->args);
    fcFollow fruit = cxActionView(e->sender);
    fcSprite sprite = cxFollowTarget(e->sender);
    //sprite 被 fruit 击中，凶手是attacker
    if(!fcSpriteHasOver(sprite)){
        CX_METHOD_RUN(sprite->Attacked, sprite, fruit, attacker);
    }
    //移除水果导弹
    fcFruitRemoved(fruit);
}

//目标死亡后停止
static void fcFollowStep(cxEvent *e)
{
    fcSprite sprite = cxFollowTarget(e->sender);
    if(fcSpriteHasOver(sprite)) {
        cxActionStop(e->sender);
    }
}

static void fcFollowFire(cxAny fruit, cxAny attacker,cxAny target)
{
    fcFollow this = fruit;
    cxViewSetPos(this, cxViewPosition(attacker));
    cxViewAppend(this->super.map, this);
    cxFloat speed = fcMapScaleValue(this->super.map, this->super.speed);
    cxFollow follow = cxFollowCreate(speed, target);
    CX_METHOD_OVERRIDE(follow->Exit, fcFollowExit);
    CX_EVENT_APPEND(follow->super.onStop, fcFollowStop, cxEventArgWeakRef(attacker));
    CX_EVENT_APPEND(follow->super.onStep, fcFollowStep, cxEventArgWeakRef(attacker));
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