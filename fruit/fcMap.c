//
//  fcMap.c
//  dome
//
//  Created by xuhua on 3/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//
#include <algorithm/cxAStar.h>
#include <core/cxEngine.h>
#include "fcMap.h"
#include "fcThrower.h"
#include "fcFruit.h"
#include "fcFollow.h"
#include "fcSpriteMove.h"
#include "fcTreasureBox.h"
#include "fcSpeedUp.h"
#include "fcIntruder.h"
#include "fcLocation.h"

CX_LUA_METHOD_BEG(fcMap)

CX_LUA_METHOD_END(fcMap)

void __fcMapTypeInit()
{
    CX_LUA_LOAD_TYPE(fcMap);
}

static void fcPathNodeNeighbors(ASNeighborList neighbors, void *node, void *context)
{
    fcMap map = context;
    cxVec2i *pathNode = (cxVec2i *)node;
    //left
    cxVec2i l = cxVec2iv(pathNode->x - 1, pathNode->y);
    cxInt lv = fcMapValue(map, l);
    if(lv == 0){
        ASNeighborListAdd(neighbors, &l, 1);
    }
    //right
    cxVec2i r = cxVec2iv(pathNode->x + 1, pathNode->y);
    cxInt rv = fcMapValue(map, r);
    if(rv == 0){
        ASNeighborListAdd(neighbors, &r, 1);
    }
    //up
    cxVec2i u = cxVec2iv(pathNode->x, pathNode->y + 1);
    cxInt uv = fcMapValue(map, u);
    if(uv == 0){
        ASNeighborListAdd(neighbors, &u, 1);
    }
    //down
    cxVec2i d = cxVec2iv(pathNode->x, pathNode->y - 1);
    cxInt dv = fcMapValue(map, d);
    if(dv == 0){
        ASNeighborListAdd(neighbors, &d, 1);
    }
    //left up
    cxVec2i lu = cxVec2iv(pathNode->x - 1, pathNode->y + 1);
    cxInt luv = fcMapValue(map, lu);
    if(luv == 0 && lv == 0 && uv == 0){
        ASNeighborListAdd(neighbors, &lu, 1);
    }
    //right up
    cxVec2i ru = cxVec2iv(pathNode->x + 1, pathNode->y + 1);
    cxInt ruv = fcMapValue(map, ru);
    if(ruv == 0 && rv == 0 && uv == 0){
        ASNeighborListAdd(neighbors, &ru, 1);
    }
    //left down
    cxVec2i ld = cxVec2iv(pathNode->x - 1, pathNode->y - 1);
    cxInt ldv = fcMapValue(map, ld);
    if(ldv == 0 && lv == 0 && dv == 0){
        ASNeighborListAdd(neighbors, &ld, 1);
    }
    //right down
    cxVec2i rd = cxVec2iv(pathNode->x + 1, pathNode->y - 1);
    cxInt rdv = fcMapValue(map, rd);
    if(rdv == 0 && rv == 0 && dv == 0){
        ASNeighborListAdd(neighbors, &rd, 1);
    }
}

static int fcPathEarlyExit(size_t visitedCount, void *visitingNode, void *goalNode, void *context)
{
    return 0;
}

static float fcPathNodeHeuristic(void *fromNode, void *toNode, void *context)
{
    cxVec2i *from = (cxVec2i *)fromNode;
    cxVec2i *to = (cxVec2i *)toNode;
    return (fabs(from->x - to->x) + fabs(from->y - to->y));
}

static const ASPathNodeSource PathNodeSource =
{
    sizeof(cxVec2i),
    &fcPathNodeNeighbors,
    &fcPathNodeHeuristic,
    &fcPathEarlyExit,
    NULL
};

cxBool fcMapCheckIdx(cxVec2i idx)
{
    return idx.x >= 0 && idx.x < DM_MAP_WIDTH && idx.y >= 0 && idx.y < DM_MAP_HEIGHT;
}

cxAny fcMapSprite(fcMap this,cxVec2i idx)
{
    if(!fcMapCheckIdx(idx)){
        return NULL;
    }
    return this->sprites[idx.x][idx.y];
}

cxInt fcMapValue(fcMap this,cxVec2i idx)
{
    fcSprite sprite = fcMapSprite(this, idx);
    if(sprite == NULL){
        return 0;
    }
    return fcSpritePathValue(sprite);
}

cxVec2i fcMapToIdx(fcMap this,cxVec2f pos)
{
    cxSize2f size = cxViewSize(this);
    cxFloat x = pos.x + size.w / 2.0f;
    cxFloat y = pos.y + size.h / 2.0f;
    return cxVec2iv(x/this->gridSize.w, y/this->gridSize.h);
}

cxVec2f fcMapToPos(fcMap this,cxVec2i idx)
{
    cxSize2f size = cxViewSize(this);
    cxFloat x = idx.x * this->gridSize.w - size.w / 2.0f;
    cxFloat y = idx.y * this->gridSize.h - size.h / 2.0f;
    return cxVec2fv(x + this->gridSize.w/2.0f, y + this->gridSize.h/2.0f);
}

static cxBool fcMapTouch(cxAny pview,cxTouch *touch)
{
    fcMap this = pview;
    cxVec2f pos;
    if(!cxViewHitTest(this, touch->current, &pos)){
        return false;
    }
    return false;
}

cxFloat fcMapScaleValue(fcMap this,cxFloat v)
{
    return this->gridSize.w * v;
}

cxBool fcMapFindPath(fcMap this,fcPath *path,cxVec2i start,cxVec2i stop)
{
    CX_ASSERT(path != NULL && this != NULL, "map or path error");
    cxBool rv = false;
    ASPath apath = ASPathCreate(&PathNodeSource, this, &start, &stop);
    fcPathClean(path);
    for (int i=0; i<ASPathGetCount(apath); i++) {
        cxVec2i *pathNode = ASPathGetNode(apath, i);
        fcPathAppend(path, *pathNode);
    }
    rv = (ASPathGetCount(apath) > 0);
    ASPathDestroy(apath);
    return rv;
}

void fcMapAppendProps(fcMap this,cxAny sprite)
{
    fcSprite m = sprite;
    m->element = cxListAppend(this->props, sprite);
    cxViewAppend(this, sprite);
}

void fcMapRemoveProps(fcMap this,cxAny sprite)
{
    fcSprite m = sprite;
    if(m->element != NULL){
        cxListRemove(this->props, m->element);
        cxViewRemoved(m);
        m->element = NULL;
    }
}

void fcMapAppendIntruder(fcMap this,cxAny sprite)
{
    fcSprite m = sprite;
    m->element = cxListAppend(this->intruder, sprite);
    cxViewAppend(this, sprite);
}

void fcMapRemoveIntruder(fcMap this,cxAny sprite)
{
    fcSprite m = sprite;
    if(m->element != NULL){
        cxListRemove(this->intruder, m->element);
        cxViewRemoved(m);
        m->element = NULL;
    }
}

void fcMapAppendDefenser(fcMap this,cxAny sprite)
{
    fcSprite m = sprite;
    m->element = cxListAppend(this->defenser, sprite);
    cxViewAppend(this, sprite);
}

void fcMapRemoveDefenser(fcMap this,cxAny sprite)
{
    fcSprite m = sprite;
    if(m->element != NULL){
        cxListRemove(this->defenser, m->element);
        cxViewRemoved(m);
        m->element = NULL;
    }
}

static void attackedTest(cxAny sprite,cxAny fruit,cxAny attacker)
{
//    fcThrower s = sprite;
//    fcFruit f = fruit;
//    CX_LOGGER("%p attacked %p,attacker = %p",f, s, attacker);
//    fcThrowerPauseTime(attacker,5);
//    fcSpriteRemoved(s);
}

static cxBool isAttackMe(cxAny sprite, cxAny fruit,cxAny attacker)
{
//    fcSprite s = sprite;
//    fcFruit f = fruit;
//    CX_LOGGER("%p will attacked %p,use fruit = %p",attacker, s, f);
    return true;
}

void fcMapSetMode(cxAny this,fcMapMode mode)
{
    fcMap m = this;
    m->mode = mode;
}

cxVec2i fcMapBeginLocation(cxAny map)
{
    fcMap this = map;
    CX_ASSERT(this->bLoc != NULL, "not set begin location");
    return fcSpriteIndex(this->bLoc);
}

cxVec2i fcMapEndLocation(cxAny map)
{
    fcMap this = map;
    CX_ASSERT(this->eLoc != NULL, "not set end location");
    return fcSpriteIndex(this->eLoc);
}

static void fcMapLocation(cxAny map,cxAny loc)
{
    fcMap this = map;
    fcLocation l = loc;
    if(l->type != fcLocationTypeBegin){
        return;
    }
    fcSprite b = CX_CREATE(fcSprite);
    fcSpriteInit(b, this);
    fcSpriteInitIndex(b, fcMapBeginLocation(this), false);
    cxSpriteSetImage(b, "item.xml?blue.png");
    CX_METHOD_OVERRIDE(b->IsAttack, isAttackMe);
    CX_METHOD_OVERRIDE(b->Attacked, attackedTest);
    b->speed = 2;
    fcMapAppendIntruder(this, b);
    fcSpriteMoveLoop(b);
    fcIntruderLoop(b);
}

void fcMapReadAttr(cxReaderAttrInfo *info)
{
    cxViewReadAttr(info);
}

CX_OBJECT_INIT(fcMap, cxView)
{
    cxObjectSetReadAttrFunc(this, fcMapReadAttr);
    fcMapSetMode(this, fcMapModeEdit);
    //
    CX_METHOD_OVERRIDE(this->Location, fcMapLocation);
    //
    this->intruder = CX_ALLOC(cxList);
    this->defenser = CX_ALLOC(cxList);
    this->props = CX_ALLOC(cxList);
    //
    cxSize2f size = cxEngineInstance()->winsize;
    cxFloat vw = size.w - 20;
    this->gridSize.w = vw / DM_MAP_WIDTH;
    this->gridSize.h = this->gridSize.w;
    cxViewSetSize(this, cxSize2fv(vw, vw));
    //
    CX_METHOD_OVERRIDE(this->super.Touch, fcMapTouch);
    //设置起点和终点
    {
        this->bLoc = CX_CREATE(fcLocation);
        fcLocationInit(this->bLoc, this, fcLocationTypeBegin);
        fcSpriteInitIndex(this->bLoc, cxVec2iv(0, 1), true);
        cxSpriteSetImage(this->bLoc, "item.xml?begin.png");
        fcMapAppendDefenser(this, this->bLoc);
        
        this->eLoc = CX_CREATE(fcLocation);
        fcLocationInit(this->eLoc, this, fcLocationTypeEnd);
        fcSpriteInitIndex(this->eLoc, cxVec2iv(9, 8), true);
        cxSpriteSetImage(this->eLoc, "item.xml?end.png");
        fcMapAppendDefenser(this, this->eLoc);
    }
    //
    {
        fcThrower a = CX_CREATE(fcThrower);
        fcThrowerInit(a, this, fcThrowerTypeSmallMachine);
        fcSpriteInitIndex(a, cxVec2iv(5, 8), true);
        cxSpriteSetImage(a, "item.xml?red.png");
        CX_METHOD_OVERRIDE(a->FruitMaker, fcFollowMaker);
        a->attackRange = 4;
        a->attackNumber = 2;
        a->attackPower = 5;
        fcMapAppendDefenser(this, a);
        //开始搜索攻击
        fcThrowerLoop(a);
    }
    
    {
        fcThrower a = CX_CREATE(fcThrower);
        fcThrowerInit(a, this, fcThrowerTypeSmallMachine);
        fcSpriteInitIndex(a, cxVec2iv(7, 6), true);
        cxSpriteSetImage(a, "item.xml?red.png");
        CX_METHOD_OVERRIDE(a->FruitMaker, fcFollowMaker);
        a->attackRange = 4;
        a->attackNumber = 2;
        a->attackPower = 5;
        fcMapAppendDefenser(this, a);
        //开始搜索攻击
        fcThrowerLoop(a);
    }
    
    {
        fcThrower a = CX_CREATE(fcThrower);
        fcThrowerInit(a, this, fcThrowerTypeSmallMachine);
        fcSpriteInitIndex(a, cxVec2iv(5, 5), true);
        cxSpriteSetImage(a, "item.xml?red.png");
        CX_METHOD_OVERRIDE(a->FruitMaker, fcFollowMaker);
        a->attackRange = 4;
        a->attackNumber = 2;
        a->attackPower = 5;
        fcMapAppendDefenser(this, a);
        //开始搜索攻击
        fcThrowerLoop(a);
    }
    
    {
        fcThrower a = CX_CREATE(fcThrower);
        fcThrowerInit(a, this, fcThrowerTypeSmallMachine);
        fcSpriteInitIndex(a, cxVec2iv(3, 4), true);
        cxSpriteSetImage(a, "item.xml?red.png");
        CX_METHOD_OVERRIDE(a->FruitMaker, fcFollowMaker);
        a->attackRange = 4;
        a->attackNumber = 2;
        a->attackPower = 5;
        fcMapAppendDefenser(this, a);
        //开始搜索攻击
        fcThrowerLoop(a);
    }
    
    {
        fcTreasureBox b = CX_CREATE(fcTreasureBox);
        cxSpriteSetImage(b, "item.xml?white.png");
        fcTreasureBoxInit(b, this);
        fcSpriteInitIndex(b, cxVec2iv(1, 2), true);
        fcMapAppendDefenser(this, b);
    }
    {
        fcSpeedUp su = CX_CREATE(fcSpeedUp);
        cxSpriteSetImage(su, "item.xml?yellow.png");
        fcSpriteInit(su, this);
        fcSpriteInitIndex(su, cxVec2iv(8,8), true);
        fcMapAppendProps(this, su);
    }
}
CX_OBJECT_FREE(fcMap, cxView)
{
    CX_METHOD_RELEASE(this->Arrive);
    CX_METHOD_RELEASE(this->Location);
    CX_RELEASE(this->defenser);
    CX_RELEASE(this->props);
    CX_RELEASE(this->intruder);
}
CX_OBJECT_TERM(fcMap, cxView)










