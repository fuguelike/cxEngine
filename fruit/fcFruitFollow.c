//
//  fcFruitFollow.c
//  fruit
//
//  Created by xuhua on 3/26/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxFollow.h>
#include "fcMap.h"
#include "fcSprite.h"
#include "fcFruitFollow.h"

static void fcFruitFollowStop(cxEvent *e)
{
    fcFruitFollow this = cxActionView(e->sender);
    fcSprite target = cxFollowTarget(e->sender);
    //目标被攻击了
    CX_METHOD_FIRE(0, target->Attacked, target, this);
    //默认移除
    fcFruitRemoved(this);
}

static cxInt fcFruitFollowFire(cxAny this, cxAny sprite, cxAny target)
{
    fcFruitFollow ff = this;
    cxViewSetPos(this, cxViewPosition(sprite));
    cxViewAppend(ff->super.map, this);
    cxFloat speed = fcMapScaleValue(ff->super.map, ff->super.speed);
    cxFollow follow = cxFollowCreate(speed, target);
    CX_EVENT_QUICK(follow->super.onStop, fcFruitFollowStop);
    cxViewAppendAction(ff, follow);
    return 0;
}

CX_OBJECT_INIT(fcFruitFollow, fcFruit)
{
    this->super.attackPower = 10;
    this->super.type = fcFruitTypeFollow;
    CX_METHOD_OVERRIDE(this->super.Fire, fcFruitFollowFire);
}
CX_OBJECT_FREE(fcFruitFollow, fcFruit)
{

}
CX_OBJECT_TERM(fcFruitFollow, fcFruit)

fcFruitFollow fcFruitFollowCreate(cxAny map,cxFloat speed)
{
    fcFruitFollow this = CX_CREATE(fcFruitFollow);
    fcFruitInit(this, map, "item.xml?fire.png",speed);
    return this;
}