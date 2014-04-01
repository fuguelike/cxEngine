//
//  fcIntruder.c
//  fruit
//
//  Created by xuhua on 3/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcMap.h"
#include "fcIntruder.h"

CX_OBJECT_INIT(fcIntruder, fcSprite)
{
    this->rate = 0.1f;
    this->super.type = fcSpriteTypeIntruder;
}
CX_OBJECT_FREE(fcIntruder, fcSprite)
{
    
}
CX_OBJECT_TERM(fcIntruder, fcSprite)

void fcIntruderInit(cxAny this,cxAny map,fcIntruderType type)
{
    fcIntruder i = this;
    i->type = type;
    fcSpriteInit(this, map);
}

static void fcIntruderRun(cxEvent *e)
{
    fcIntruder this = cxActionView(e->sender);
    fcMap map = this->super.map;
    CX_LIST_FOREACH(map->props, ele){
        fcSprite target = ele->any;
        CX_ASSERT(target->type == fcSpriteTypeProperty, "must is property");
        cxSize2f ts = cxViewSize(target);
        cxSize2f cs = cxViewSize(this);
        cxFloat min = CX_MIN(ts.h, ts.w)/2.0f + CX_MIN(cs.h, cs.w)/2.0f;
        cxFloat distance = fcSpriteDistance(this, target);
        //target被this碰到
        if(distance < min){
            CX_METHOD_RUN(target->Collide, target, this);
        }
    }
}

void fcIntruderLoop(cxAny this)
{
    fcIntruder i = this;
    fcSpriteStartAILoop(this, fcIntruderRun, i->rate);
}

