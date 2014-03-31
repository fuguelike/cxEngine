//
//  fcThrower.c
//  fruit
//
//  Created by xuhua on 3/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxFollow.h>
#include "fcThrower.h"
#include "fcMap.h"
#include "fcFollow.h"

//target 是否在攻击范围内
static cxBool fcThrowerInRange(cxAny this,cxAny target)
{
    if(this == target){
        return false;
    }
    fcThrower s = this;
    fcSprite t = target;
    cxFloat r = fcMapScaleValue(s->super.map, s->attackRange);
    cxFloat d = fcSpriteDistance(s, t);
    return d <= r;
}

CX_OBJECT_INIT(fcThrower, fcSprite)
{
    this->super.type = fcSpriteTypeThrower;
    this->attackRange = 0;
    this->attackRate = 0.5f;
    this->attackPower = 0;
}
CX_OBJECT_FREE(fcThrower, fcSprite)
{
    CX_METHOD_RELEASE(this->FruitFire);
    CX_METHOD_RELEASE(this->FruitMaker);
}
CX_OBJECT_TERM(fcThrower, fcSprite)

//搜索敌人开始
static void fcThrowerSearchTarget(fcThrower this)
{
    fcMap map = this->super.map;
    CX_LIST_FOREACH_SAFE(map->intruder, ele, tmp){
        fcSprite target = ele->any;
        //已经在目标列表中
        if(fcSpriteHasTarget(this, target)){
            continue;
        }
        //不在攻击范围内
        if(!fcThrowerInRange(this, target)){
            continue;
        }
        fcSpriteTarget(this, target);
        //到达攻击数量
        if(this->attackNumber == fcSpriteTargetNumber(this)){
            break;
        }
    }
}

//sprite 使用  fruit 投掷 target
static void fcThrowerFire(cxAny sprite, cxAny fruit,cxAny target)
{
    fcThrower a = sprite;
    fcFruit this = fruit;
    //盯住目标
    fcSpriteLookAt(a, target);
    //投掷水果
    CX_METHOD_RUN(this->Fire, this, a, target);
    //
    CX_METHOD_RUN(a->FruitFire, a, fruit, target);
}

static void fcThrowerRun(cxEvent *e)
{
    fcThrower this = cxActionView(e->sender);
    cxIndex targetNum = fcSpriteTargetNumber(this);
    //没有目标就搜索范围内的目标
    if(targetNum < this->attackNumber){
        fcThrowerSearchTarget(this);
    }
    //攻击目标
    CX_HASH_FOREACH(this->super.targets, ele, tmp){
        fcSprite sprite = ele->any;
        //解除范围外的目标
        if(!fcThrowerInRange(this,sprite)){
            fcSpriteUnset(sprite);
            continue;
        }
        //创建水果弹药发射
        cxAny fruit = CX_METHOD_GET(NULL, this->FruitMaker, this);
        if(fruit == NULL){
            continue;
        }
        //sprite 是否能被 this 投掷的 fruit 攻击
        if(!CX_METHOD_GET(true, sprite->IsAttack, sprite, fruit, this)){
            continue;
        }
        fcThrowerFire(this, fruit, sprite);
    }
}

void fcThrowerInit(cxAny this,cxAny map,fcThrowerType type)
{
    fcThrower t = this;
    t->type = type;
    fcSpriteInit(this, map);
}

void fcThrowerLoop(cxAny this)
{
    fcThrower a = this;
    CX_ASSERT(a->attackNumber > 0, "can attack when attack number > 0");
    CX_ASSERT(a->attackRate > 0, "can attack when attack rate > 0");
    CX_ASSERT(a->attackPower > 0, "can attack when attack power > 0");
    CX_ASSERT(a->attackRange > 0, "can attack when attack range > 0");
    fcSpriteStartAITimer(a, fcThrowerRun, a->attackRate);
}



