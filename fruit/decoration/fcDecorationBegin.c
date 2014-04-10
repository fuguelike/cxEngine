//
//  fcDecoration.c
//  fruit
//
//  Created by xuhua on 4/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <fcMap.h>
#include <fcFruit.h>
#include <fcThrower.h>
#include <fcIntruder.h>
#include <fcSpriteMove.h>
#include "fcDecorationBegin.h"


//受到攻击时
static void attackedTest(cxAny sprite,cxAny fruit,cxAny attacker)
{
    fcThrower a = attacker;
    fcIntruder s = sprite;
    fcFruit f = fruit;
    
    s->super.life -= 10;
    
    if(s->super.life <= 0){
        CX_LOGGER("%p attacked %p,attacker = %p removed",f, s, a);
        fcSpriteUnset(sprite);
        fcMapRemoveIntruder(s->super.map, s);
    }
}

//是否可以攻击
static cxBool isAttackMe(cxAny sprite, cxAny fruit,cxAny attacker)
{
    fcIntruder s = sprite;
    return s->super.life > 0;
}

static void fcDecorationBeginRun(fcDecorationBegin this,fcMap map)
{
    fcIntruder b = CX_CREATE(fcIntruder);
    fcIntruderInit(b, map, 0);
    fcSpriteInitIndex(b,fcSpriteIndex(this), false);
    cxSpriteSetImage(b, "item.xml?blue.png");
    cxViewSetSize(b, cxSize2fv(map->gridSize.w/2, map->gridSize.h/2));
    CX_METHOD_OVERRIDE(b->super.IsAttack, isAttackMe);
    CX_METHOD_OVERRIDE(b->super.Attacked, attackedTest);
    b->super.speed = 2;
    b->super.life = 100;
    fcMapAppendIntruder(map, b);
    fcSpriteMoveLoop(b);
    fcIntruderLoop(b);
}

static cxBool fcDecorationBeginTouch(cxAny pview,cxTouch *touch)
{
    fcDecorationBegin this = pview;
    fcMap map = fcSpriteMapView(pview);
    cxVec2f pos = cxVec2fv(0, 0);
    if(fcSpriteTouch(pview, touch)){
        return true;
    }
    if(map->mode != fcMapModeFire){
        return false;
    }
    if(!cxViewHitTest(pview, touch->current, &pos)){
        cxActionPause(this->timer);
        return false;
    }
    if(touch->type == cxTouchTypeDown){
        this->count = 0;
        cxTimerReset(this->timer);
        cxActionResume(this->timer);
        return true;
    }
    if(touch->type == cxTouchTypeUp){
        cxActionPause(this->timer);
        fcDecorationBeginRun(this, map);
        return true;
    }
    return false;
}

static void fcDecorationBeginTimerArrive(cxEvent *e)
{
    fcDecorationBegin this = cxActionView(e->sender);
    fcMap map = fcSpriteMapView(this);
    if(this->count > 2){
        fcDecorationBeginRun(this, map);
    }
    this->count ++;
}

CX_OBJECT_INIT(fcDecorationBegin, fcDecoration)
{
    this->freq = 0.1f;
    CX_METHOD_OVERRIDE(this->super.super.super.super.Touch, fcDecorationBeginTouch);
}
CX_OBJECT_FREE(fcDecorationBegin, fcDecoration)
{
    
}
CX_OBJECT_TERM(fcDecorationBegin, fcDecoration)

void fcDecorationBeginInit(cxAny pview,cxAny map)
{
    fcDecorationBegin this = pview;
    this->timer = cxViewAppendTimer(this, this->freq, CX_TIMER_NOT_LIMIT_REPEAT);
    CX_EVENT_QUICK(this->timer->onArrive, fcDecorationBeginTimerArrive);
    cxActionPause(this->timer);
    fcDecorationInit(this, map, fcDecorationTypeBegin);
}


