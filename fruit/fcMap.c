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
#include "fcContainer.h"
#include "fcProperty.h"
#include "fcIntruder.h"
#include "decoration/fcDecorationBegin.h"
#include "decoration/fcDecorationEnd.h"

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

static void fcMapAppendItem(fcMap this, cxList list,cxAny item)
{
    fcSprite s = item;
    s->element = cxListAppend(list, item);
    if(s->isSpace){
        CX_ASSERT(fcMapSprite(this, s->idx) == NULL, "has sprite in here");
        this->sprites[s->idx.x][s->idx.y] = s;
    }
    cxViewAppend(this, item);
}

static void fcMapRemoveItem(fcMap this, cxList list,cxAny item)
{
    fcSprite s = item;
    if(s->element == NULL){
        return;
    }
    if(s->isSpace){
        CX_ASSERT(fcMapSprite(this, s->idx) != NULL, "has sprite in here");
        this->sprites[s->idx.x][s->idx.y] = NULL;
    }
    cxListRemove(list, s->element);
    s->element = NULL;
    cxViewRemoved(s);
}

void fcMapAppendProps(fcMap this,cxAny sprite)
{
    fcMapAppendItem(this, this->props, sprite);
}

void fcMapRemoveProps(fcMap this,cxAny sprite)
{
    fcMapRemoveItem(this, this->props, sprite);
}

void fcMapAppendDecoration(fcMap this,cxAny sprite)
{
    fcMapAppendItem(this, this->decoration, sprite);
}

void fcMapRemoveDecoration(fcMap this,cxAny sprite)
{
    fcMapRemoveItem(this, this->decoration, sprite);
}

void fcMapAppendIntruder(fcMap this,cxAny sprite)
{
    fcMapAppendItem(this, this->intruder, sprite);
}

void fcMapRemoveIntruder(fcMap this,cxAny sprite)
{
    fcMapRemoveItem(this, this->intruder, sprite);
}

void fcMapAppendDefenser(fcMap this,cxAny sprite)
{
    fcMapAppendItem(this, this->defenser, sprite);
}

void fcMapRemoveDefenser(fcMap this,cxAny sprite)
{
    fcMapRemoveItem(this, this->defenser, sprite);
}

void fcMapSetMode(cxAny this,fcMapMode mode)
{
    fcMap m = this;
    m->mode = mode;
}

cxBool fcMapHasPass(cxAny map)
{
    fcMap this = map;
    cxVec2i b = fcMapBegIndex(map);
    cxVec2i e = fcMapEndIndex(map);
    return fcMapFindPath(map, &this->path, b, e);
}

cxVec2i fcMapEndIndex(cxAny map)
{
    fcMap this = map;
    CX_ASSERT(this->end != NULL, "not set end location");
    return fcSpriteIndex(this->end);
}

cxVec2i fcMapBegIndex(cxAny map)
{
    fcMap this = map;
    CX_ASSERT(this->begin != NULL, "not set begin location");
    return fcSpriteIndex(this->begin);
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
    this->decoration    = CX_ALLOC(cxList);
    this->intruder      = CX_ALLOC(cxList);
    this->defenser      = CX_ALLOC(cxList);
    this->props         = CX_ALLOC(cxList);
    //
    cxSize2f size = cxEngineInstance()->winsize;
    cxFloat vw = size.w - 20;
    this->gridSize.w = vw / DM_MAP_WIDTH;
    this->gridSize.h = this->gridSize.w;
    cxViewSetSize(this, cxSize2fv(vw, vw));
    //
    CX_METHOD_OVERRIDE(this->super.Touch, fcMapTouch);
    //设置1个起点和1个终点(只能有1个终点)
    {
        this->begin = CX_CREATE(fcDecorationBegin);
        fcDecorationBeginInit(this->begin, this);
        fcSpriteInitIndex(this->begin, cxVec2iv(0, 0), true);
        cxSpriteSetImage(this->begin, "item.xml?begin.png");
        fcMapAppendDecoration(this, this->begin);
    }
    {
        this->end = CX_CREATE(fcDecorationEnd);
        fcDecorationEndInit(this->end, this);
        fcSpriteInitIndex(this->end, cxVec2iv(9, 8), true);
        cxSpriteSetImage(this->end, "item.xml?end.png");
        fcMapAppendDecoration(this, this->end);
    }
    //
    {
        fcThrower a = CX_CREATE(fcThrower);
        fcThrowerInit(a, this, fcThrowerTypeSmallMachine);
        fcSpriteInitIndex(a, cxVec2iv(5, 8), true);
        cxSpriteSetImage(a, "item.xml?red.png");
        CX_METHOD_OVERRIDE(a->FruitMaker, fcFollowMaker);
        a->attackRange = 4;
        a->attackNumber = 1;
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
        a->attackNumber = 1;
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
        a->attackNumber = 1;
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
        a->attackNumber = 1;
        a->attackPower = 5;
        fcMapAppendDefenser(this, a);
        //开始搜索攻击
        fcThrowerLoop(a);
    }
    
    {
        fcContainer b = CX_CREATE(fcContainer);
        cxSpriteSetImage(b, "item.xml?white.png");
        fcContainerInit(b, this, 0);
        fcSpriteInitIndex(b, cxVec2iv(1, 2), true);
        fcMapAppendDefenser(this, b);
    }
    {
        fcProperty su = CX_CREATE(fcProperty);
        cxSpriteSetImage(su, "item.xml?yellow.png");
        fcPropertyInit(su, this, 0);
        fcSpriteInitIndex(su, cxVec2iv(8,8), true);
        fcMapAppendProps(this, su);
    }
}
CX_OBJECT_FREE(fcMap, cxView)
{
    CX_RELEASE(this->defenser);
    CX_RELEASE(this->props);
    CX_RELEASE(this->intruder);
    CX_RELEASE(this->decoration);
}
CX_OBJECT_TERM(fcMap, cxView)










