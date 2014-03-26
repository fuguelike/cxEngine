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
    fcFollow this = cxActionView(e->sender);
    fcSprite target = cxFollowTarget(e->sender);
    //目标被攻击了
    CX_METHOD_FIRE(0, target->Attacked, target, this);
    //默认移除
    fcFruitRemoved(this);
}

static cxInt fcFollowFire(cxAny this, cxAny sprite, cxAny target)
{
    fcFollow ff = this;
    cxViewSetPos(this, cxViewPosition(sprite));
    cxViewAppend(ff->super.map, this);
    cxFloat speed = fcMapScaleValue(ff->super.map, ff->super.speed);
    cxFollow follow = cxFollowCreate(speed, target);
    CX_EVENT_QUICK(follow->super.onStop, fcFollowStop);
    cxViewAppendAction(ff, follow);
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