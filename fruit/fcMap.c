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
#include "fcAttacker.h"


static void PathNodeNeighbors(ASNeighborList neighbors, void *node, void *context)
{
    fcMap map = context;
    cxVec2i *pathNode = (cxVec2i *)node;
    //left
    cxVec2i left = cxVec2iv(pathNode->x - 1, pathNode->y);
    cxInt leftValue = fcMapValue(map, left);
    if(leftValue == 0){
        ASNeighborListAdd(neighbors, &left, 1);
    }
    //right
    cxVec2i right = cxVec2iv(pathNode->x + 1, pathNode->y);
    cxInt rightValue = fcMapValue(map, right);
    if(rightValue == 0){
        ASNeighborListAdd(neighbors, &right, 1);
    }
    //up
    cxVec2i up = cxVec2iv(pathNode->x, pathNode->y + 1);
    cxInt upValue = fcMapValue(map, up);
    if(upValue == 0){
        ASNeighborListAdd(neighbors, &up, 1);
    }
    //down
    cxVec2i down = cxVec2iv(pathNode->x, pathNode->y - 1);
    cxInt downValue = fcMapValue(map, down);
    if(downValue == 0){
        ASNeighborListAdd(neighbors, &down, 1);
    }
}

static int PathEarlyExit(size_t visitedCount, void *visitingNode, void *goalNode, void *context)
{
    return 0;
}

static float PathNodeHeuristic(void *fromNode, void *toNode, void *context)
{
    cxVec2i *from = (cxVec2i *)fromNode;
    cxVec2i *to = (cxVec2i *)toNode;
    return (fabs(from->x - to->x) + fabs(from->y - to->y));
}

static const ASPathNodeSource PathNodeSource =
{
    sizeof(cxVec2i),
    &PathNodeNeighbors,
    &PathNodeHeuristic,
    &PathEarlyExit,
    NULL
};

cxBool fcMapCheckIdx(cxVec2i idx)
{
    return idx.x >= 0 && idx.x < DM_MAP_WIDTH && idx.y >= 0 && idx.y < DM_MAP_HEIGHT;
}

cxInt fcMapValue(fcMap this,cxVec2i idx)
{
    if(!fcMapCheckIdx(idx)){
        return -1;
    }
    return this->values[idx.x][idx.y];
}

void fcMapSetValue(fcMap this,cxVec2i idx,cxInt value)
{
    if(fcMapCheckIdx(idx)){
        this->values[idx.x][idx.y] = value;
    }
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

fcAttacker m;

static cxBool fcMapTouch(cxAny pview,cxTouch *touch)
{
    fcMap this = pview;
    cxVec2f pos;
    if(!cxViewHitTest(this, touch->current, &pos)){
        return false;
    }
    if(touch->type == cxTouchTypeMove){
        cxViewSetPos(m, pos);
    }
//    if(touch->type == cxTouchTypeDown){
//        cxVec2i idx = fcMapToIdx(this, pos);
//        this->values[idx.x][idx.y] = 1;
//        fcPath path;
//        if(!fcMapFindPath(this, &path, cxVec2iv(0, 0), cxVec2iv(9, 9))){
//            this->values[idx.x][idx.y] = 0;
//            return false;
//        }
//        
//        fcSprite sp = fcSpriteCreate(this, idx);
//        cxViewAppend(this, sp);
//        cxViewAppend(this, sp);
//        cxObjectSetTag(sp, 1);
//        cxSpriteSetImage(sp, "item.xml?blue.png");
//        
//        CX_LIST_FOREACH_SAFE(this->super.subViews, ele, tmp){
//            cxInt tag = cxObjectTag(ele->any);
//            if(tag != 1){
//                cxViewRemoved(ele->any);
//            }
//        }
//        
//        for(cxInt i=0; i < path.number; i++){
//            cxVec2i idx = path.points[i];
//            fcSprite sp = fcSpriteCreate(this, idx);
//            cxSpriteSetImage(sp, "item.xml?white.png");
//            cxViewAppend(this, sp);
//        }
//    }
    return false;
}

cxBool fcMapFindPath(fcMap this,fcPath *path,cxVec2i start,cxVec2i stop)
{
    cxBool rv = false;
    ASPath apath = ASPathCreate(&PathNodeSource, this, &start, &stop);
    path->number = 0;
    for (int i=0; i<ASPathGetCount(apath); i++) {
        cxVec2i *pathNode = ASPathGetNode(apath, i);
        path->points[path->number++] = *pathNode;
    }
    rv = (ASPathGetCount(apath) > 0);
    ASPathDestroy(apath);
    return rv;
}

CX_OBJECT_INIT(fcMap, cxView)
{
    this->sprites = CX_ALLOC(cxArray);
    
    CX_METHOD_OVERRIDE(this->super.Touch, fcMapTouch);
    
    cxSize2f size = cxEngineInstance()->winsize;
    cxFloat vw = size.w - 20;
    this->gridSize.w = vw / DM_MAP_WIDTH;
    this->gridSize.h = this->gridSize.w;
    cxViewSetSize(this, cxSize2fv(vw, vw));
    
    fcAttacker a = fcAttackerCreate(this, cxVec2iv(0, 0), fcAttackerTypeSmallMachine);
    cxSpriteSetImage(a, "item.xml?red.png");
    cxViewAppend(this, a);
    
    fcAttacker b = fcAttackerCreate(this, cxVec2iv(6, 6), fcAttackerTypeSmallMachine);
    cxSpriteSetImage(b, "item.xml?blue.png");
    cxViewAppend(this, b);
    m = b;
    
    fcSpriteTarget(a, b);
}
CX_OBJECT_FREE(fcMap, cxView)
{
    CX_RELEASE(this->sprites);
}
CX_OBJECT_TERM(fcMap, cxView)










