//
//  fcAttacker.c
//  fruit
//
//  Created by xuhua on 3/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxFollow.h>
#include "fcAttacker.h"
#include "fcMap.h"
#include "fcFollow.h"

//target 是否在攻击范围内
static cxBool fcAttackerInRange(cxAny this,cxAny target)
{
    if(this == target){
        return false;
    }
    fcAttacker s = this;
    fcSprite t = target;
    cxFloat r = fcMapScaleValue(s->super.map, s->attackRange);
    cxFloat d = fcSpriteDistance(s, t);
    return d <= r;
}

void fcAttackerPauseTime(cxAny this,cxFloat time)
{
    fcAttacker a = this;
    if(a->loopTimer != NULL){
        cxActionSetPauseTime(a->loopTimer, time);
    }
}

void fcAttackerPause(cxAny this)
{
    fcAttacker a = this;
    if(a->loopTimer != NULL){
        cxActionPause(a->loopTimer);
    }
}

void fcAttackerResume(cxAny this)
{
    fcAttacker a = this;
    if(a->loopTimer != NULL){
        cxActionResume(a->loopTimer);
    }
}

CX_OBJECT_INIT(fcAttacker, fcSprite)
{
    this->super.type = fcSpriteTypeAttacker;
    this->attackRange = 0;
    this->attackRate = 0.5f;
    this->attackPower = 0;
}
CX_OBJECT_FREE(fcAttacker, fcSprite)
{
    CX_METHOD_RELEASE(this->FruitMaker);
}
CX_OBJECT_TERM(fcAttacker, fcSprite)

//搜索敌人开始
static void fcAttackerSearchTarget(fcAttacker this)
{
    fcMap map = this->super.map;
    CX_LIST_FOREACH(map->fights, ele){
        fcSprite target = ele->any;
        //不攻击同一组的
        if(target->group == this->super.group){
            continue;
        }
        //已经在目标列表中
        if(fcSpriteHasTarget(this, target)){
            continue;
        }
        //不在攻击范围内
        if(!fcAttackerInRange(this, target)){
            continue;
        }
        fcSpriteTarget(this, target);
        //到达攻击数量
        if(this->attackNumber == fcSpriteTargetNumber(this)){
            break;
        }
    }
}

//sprite 使用  fruit 攻击 target
static void fcAttackerFire(cxAny sprite, cxAny fruit,cxAny target)
{
    fcFruit this = fruit;
    //盯住目标
    fcSpriteLookAt(sprite, target);
    //发射
    CX_METHOD_RUN(this->Fire, this, sprite, target);
}

static void fcAttackerRun(cxEvent *e)
{
    fcAttacker this = cxActionView(e->sender);
    cxIndex targetNum = fcSpriteTargetNumber(this);
    //没有目标就搜索范围内的目标
    if(targetNum < this->attackNumber){
        fcAttackerSearchTarget(this);
    }
    //攻击目标
    CX_HASH_FOREACH(this->super.targets, ele, tmp){
        fcSprite sprite = ele->any;
        //解除范围外的目标
        if(!fcAttackerInRange(this,sprite)){
            fcSpriteUnset(sprite);
            continue;
        }
        //创建水果弹药发射
        cxAny fruit = CX_METHOD_GET(NULL, this->FruitMaker, this);
        if(fruit == NULL){
            continue;
        }
        //sprite 是否能被 this 发射的 fruit 攻击
        cxBool isAttack = CX_METHOD_GET(true, sprite->IsAttack, sprite, fruit, this);
        if(!isAttack){
            continue;
        }
        fcAttackerFire(this, fruit, sprite);
    }
}

void fcAttackerLoop(cxAny this)
{
    fcAttacker a = this;
    CX_ASSERT(a->attackNumber > 0, "can attack when attack number > 0");
    CX_ASSERT(a->attackRate > 0, "can attack when attack rate > 0");
    CX_ASSERT(a->attackPower > 0, "can attack when attack power > 0");
    CX_ASSERT(a->attackRange > 0, "can attack when attack range > 0");
    a->loopTimer = fcSpriteTimer(this, a->attackRate);
    CX_EVENT_APPEND(a->loopTimer->onArrive, fcAttackerRun, NULL);
}

fcAttacker fcAttackerCreate(cxAny map,cxVec2i idx,fcAttackerType type)
{
    fcAttacker this = CX_CREATE(fcAttacker);
    fcSpriteInit(this, map, idx);
    this->attackerType = type;
    return this;
}