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
    fcMap map = s->map;
    //解除所有关系
    fcSpriteUnset(s);
    //移除二维索引
    if(fcMapCheckIdx(s->idx)){
        map->sprites[s->idx.x][s->idx.y] = NULL;
    }
    //移除地图
    fcMapRemoveFights(s->map, this);
}

cxInt fcSpritePathValue(cxAny this)
{
    fcSprite s = this;
    return CX_METHOD_GET(s->type, s->PathValue, s);
}

void fcSpriteSetGroup(cxAny this,fcGroupType group)
{
    fcSprite s = this;
    s->group = group;
}

void fcSpriteInitIndex(cxAny this, cxVec2i idx)
{
    CX_ASSERT(fcMapCheckIdx(idx), "idx error");
    fcSprite s = this;
    fcMap map = s->map;
    CX_ASSERT(fcMapSprite(map, idx) == NULL, "has sprite in here");
    s->idx = idx;
    map->sprites[idx.x][idx.y] = this;
    cxVec2f pos = fcMapToPos(map, idx);
    cxViewSetPos(s, pos);
}
//搜索将重新确定this的idx坐标
cxBool fcSpriteFindPath(cxAny this,cxVec2i idx)
{
    fcSprite s = this;
    s->idx = fcMapToIdx(s->map, cxViewPosition(s));
    CX_ASSERT(fcMapCheckIdx(s->idx), "must first fcSpriteInitIndex");
    return fcMapFindPath(s->map, &s->path, s->idx, idx);
}

void fcSpriteMoveTo(cxAny this,cxVec2i idx)
{
    CX_ASSERT(fcMapCheckIdx(idx), "idx error");
    fcSprite s = this;
    CX_ASSERT(!cxVec2iEqu(s->idx, idx),"self move to self");
    fcMap map = s->map;
    CX_ASSERT(fcMapSprite(map, idx) == NULL,"target idx has sprite");
    map->sprites[s->idx.x][s->idx.y] = NULL;
    map->sprites[idx.x][idx.y] = s;
    s->idx = idx;
    cxVec2f pos = fcMapToPos(map, idx);
    cxViewSetPos(s, pos);
}

CX_OBJECT_INIT(fcSprite, cxSprite)
{
    this->idx = cxVec2iv(-1, -1);
    this->targets = CX_ALLOC(cxHash);
    this->murderers = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(fcSprite, cxSprite)
{
    CX_METHOD_RELEASE(this->PathValue);
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
    cxHashKey key = cxHashPtrKey(s);
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

//target是否已经在this的攻击队列中
cxBool fcSpriteHasTarget(cxAny this,cxAny target)
{
    fcSprite s = this;
    fcSprite t = target;
    cxHashKey tk = cxHashPtrKey(t);
    return cxHashHas(s->targets, tk);
}

void fcSpriteTarget(cxAny this,cxAny target)
{
    //自己不能打自己
    CX_ASSERT(this != target, "target can't this");
    fcSprite s = this;
    fcSprite t = target;
    cxHashKey sk = cxHashPtrKey(s);
    cxHashKey tk = cxHashPtrKey(t);
    //设置目标的凶手我自己
    cxHashSet(t->murderers, sk, s);
    //设置我的目标为target
    cxHashSet(s->targets, tk, t);
}

void fcSpriteInit(cxAny sprite,cxAny map)
{
    fcSprite this = sprite;
    fcMap m = map;
    this->map = m;
    cxViewSetSize(this, m->gridSize);
}

cxTimer fcSpriteTimer(cxAny this,cxFloat time)
{
    return cxViewAppendTimer(this, time, -1);
}

