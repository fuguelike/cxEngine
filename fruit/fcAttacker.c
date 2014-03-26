//
//  fcAttacker.c
//  fruit
//
//  Created by xuhua on 3/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxFollow.h>
#include "fcAttacker.h"

CX_OBJECT_INIT(fcAttacker, fcSprite)
{
    this->super.type = fcSpriteTypeAttacker;
}
CX_OBJECT_FREE(fcAttacker, fcSprite)
{

}
CX_OBJECT_TERM(fcAttacker, fcSprite)

static void fireActionStop(cxEvent *e)
{
    cxSprite this = cxActionView(e->sender);
    fcSprite target = cxFollowTarget(e->sender);
    fcSpriteUnsetTarget(target);
    cxViewRemoved(this);
}

static void fcAttackerRun(cxEvent *e)
{
    fcAttacker this = cxActionView(e->sender);
    //没有目标就搜索范围内的目标
    //攻击目标
    CX_HASH_FOREACH(this->super.targets, ele, tmp){
        
        cxSprite fire = cxSpriteCreateWithURL("item.xml?fire.png");
        cxViewSetPos(fire, cxViewPosition(this));
        cxViewAppend(this->super.map, fire);
        
        cxFollow follow = cxFollowCreate(500, ele->any);
        cxViewAppendAction(fire, follow);
        CX_EVENT_APPEND(follow->super.onStop, fireActionStop, NULL);
    }
}

fcAttacker fcAttackerCreate(cxAny map,cxVec2i idx,fcAttackerType type)
{
    fcAttacker this = CX_CREATE(fcAttacker);
    fcSpriteInit(this, map, idx);
    this->attackerType = type;
    
    cxTimer timer = fcSpriteTimer(this, 0.5f);
    CX_EVENT_APPEND(timer->onArrive, fcAttackerRun, NULL);
    
    return this;
}