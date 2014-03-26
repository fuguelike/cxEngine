//
//  dmSprite.c
//  dome
//
//  Created by xuhua on 3/21/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcMap.h"
#include "fcSprite.h"

void fcSpriteRemoved(cxAny this)
{
    fcSprite s = this;
    CX_ASSERT(s->map != NULL, "not join map");
    //解除所有关系
    fcSpriteUnset(s);
    //移除地图
    fcMapRemoveSprite(s->map, this);
}

CX_OBJECT_INIT(fcSprite, cxSprite)
{
    this->targets = CX_ALLOC(cxHash);
    this->murderers = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(fcSprite, cxSprite)
{
    CX_METHOD_RELEASE(this->Attacked);
    CX_RELEASE(this->targets);
    CX_RELEASE(this->murderers);
}
CX_OBJECT_TERM(fcSprite, cxSprite)

cxInt fcSpriteTargetNumber(cxAny this)
{
    fcSprite s = this;
    return cxHashLength(s->targets);
}

cxInt fcSpriteMurdererNumber(cxAny this)
{
    fcSprite s = this;
    return cxHashLength(s->murderers);
}

void fcSpriteLookAt(cxAny s1,cxAny s2)
{
    CX_ASSERT(s1 != NULL && s2 != NULL, "s1 or s2 null");
    cxVec2f p1 = cxViewPosition(s1);
    cxVec2f p2 = cxViewPosition(s2);
    cxFloat angle = atan2f(p2.y - p1.y, p2.x - p1.x);
    cxViewSetAngle(s1, angle);
}

cxFloat fcSpriteDistance(cxAny s1,cxAny s2)
{
    CX_ASSERT(s1 != NULL && s2 != NULL, "s1 or s2 null");
    cxVec2f p1 = cxViewPosition(s1);
    cxVec2f p2 = cxViewPosition(s2);
    return kmVec2DistanceBetween(&p1, &p2);
}

void fcSpriteUnset(cxAny this)
{
    fcSprite s = this;
    cxHashKey key = cxHashIntKey((cxInt)s);
    //解除目标是我
    CX_HASH_FOREACH(s->targets, sele, stmp){
        fcSprite st = sele->any;
        cxHashDel(st->murderers, key);
    }
    //解除我的所有的目标
    cxHashClean(s->targets);
    //解除凶手的目标是我的
    CX_HASH_FOREACH(s->murderers, tele, ttmp){
        fcSprite tt = tele->any;
        cxHashDel(tt->targets,key);
    }
    //解除我的所有的凶手
    cxHashClean(s->murderers);
}

void fcSpriteTarget(cxAny this,cxAny target)
{
    CX_ASSERT(this != target, "target can't this");
    fcSprite s = this;
    fcSprite t = target;
    cxHashKey sk = cxHashIntKey((cxInt)s);
    cxHashKey tk = cxHashIntKey((cxInt)t);
    //设置目标的凶手我自己
    cxHashSet(t->murderers, sk, s);
    //设置我的目标为target
    cxHashSet(s->targets, tk, t);
}

void fcSpriteInit(cxAny sprite,cxAny map,cxVec2i idx)
{
    fcMap dm = map;
    fcSprite this = sprite;
    cxVec2f pos = fcMapToPos(dm, idx);
    cxViewSetPos(this, pos);
    cxViewSetSize(this, dm->gridSize);
    this->map = map;
    this->idx = idx;
}

cxTimer fcSpriteTimer(cxAny this,cxFloat time)
{
    return cxViewAppendTimer(this, time, -1);
}

