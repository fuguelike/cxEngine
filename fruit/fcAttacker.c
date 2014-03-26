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

cxBool fcAttackerIsRangeIn(cxAny this,cxAny target)
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

CX_OBJECT_INIT(fcAttacker, fcSprite)
{
    this->super.type = fcSpriteTypeAttacker;
    this->attackRange = 8;
    this->attackRate = 0.5f;
    this->attackPower = 0;
}
CX_OBJECT_FREE(fcAttacker, fcSprite)
{
    
}
CX_OBJECT_TERM(fcAttacker, fcSprite)

//搜索敌人开始
static void fcAttackerSearchTarget(fcAttacker this)
{
    fcMap map = this->super.map;
    CX_LIST_FOREACH(map->sprites, ele){
        fcSprite target = ele->any;
        //只搜索范围内的敌人
        if(target->group == this->super.group){
            continue;
        }
        //已经在目标列表中
        if(fcSpriteInTargets(this, target)){
            continue;
        }
        //不在攻击范围内
        if(!fcAttackerIsRangeIn(this, target)){
            continue;
        }
        fcSpriteTarget(this, target);
        //到达攻击数量
        if(this->attackNumber == fcSpriteTargetNumber(this)){
            break;
        }
    }
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
        //解除范围外的目标
        if(!fcAttackerIsRangeIn(this,ele->any)){
            fcSpriteUnset(ele->any);
            continue;
        }
        //创建水果弹药
        fcFollow fire = fcFollowCreate(this->super.map, 6);
        fcFruitFire(fire, this, ele->any);
    }
}

void fcAttackerLoop(cxAny this)
{
    fcAttacker a = this;
    CX_ASSERT(a->attackNumber > 0, "attacker not set attack property");
    CX_ASSERT(a->attackRate > 0, "attacker not set attack property");
    CX_ASSERT(a->attackPower > 0, "can attack when attack power > 0");
    cxTimer timer = fcSpriteTimer(this, a->attackRate);
    CX_EVENT_APPEND(timer->onArrive, fcAttackerRun, NULL);
}

fcAttacker fcAttackerCreate(cxAny map,cxVec2i idx,fcAttackerType type)
{
    fcAttacker this = CX_CREATE(fcAttacker);
    fcSpriteInit(this, map, idx);
    this->attackerType = type;
    return this;
}