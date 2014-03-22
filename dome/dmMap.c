//
//  dmMap.c
//  dome
//
//  Created by xuhua on 3/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//
#include <algorithm/cxAStar.h>
#include <core/cxEngine.h>
#include "dmMap.h"
#include "dmSprite.h"


static void PathNodeNeighbors(ASNeighborList neighbors, void *node, void *context)
{
    dmMap map = context;
    cxVec2i *pathNode = (cxVec2i *)node;
    //left
    cxVec2i left = cxVec2iv(pathNode->x - 1, pathNode->y);
    cxInt leftValue = dmMapValue(map, left);
    if(leftValue == 0){
        ASNeighborListAdd(neighbors, &left, 1);
    }
    //right
    cxVec2i right = cxVec2iv(pathNode->x + 1, pathNode->y);
    cxInt rightValue = dmMapValue(map, right);
    if(rightValue == 0){
        ASNeighborListAdd(neighbors, &right, 1);
    }
    //up
    cxVec2i up = cxVec2iv(pathNode->x, pathNode->y + 1);
    cxInt upValue = dmMapValue(map, up);
    if(upValue == 0){
        ASNeighborListAdd(neighbors, &up, 1);
    }
    //down
    cxVec2i down = cxVec2iv(pathNode->x, pathNode->y - 1);
    cxInt downValue = dmMapValue(map, down);
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

cxBool dmMapCheckIdx(cxVec2i idx)
{
    return idx.x >= 0 && idx.x < DM_MAP_WIDTH && idx.y >= 0 && idx.y < DM_MAP_HEIGHT;
}

cxInt dmMapValue(dmMap this,cxVec2i idx)
{
    if(!dmMapCheckIdx(idx)){
        return -1;
    }
    return this->values[idx.x][idx.y];
}

void dmMapSetValue(dmMap this,cxVec2i idx,cxInt value)
{
    if(dmMapCheckIdx(idx)){
        this->values[idx.x][idx.y] = value;
    }
}

cxVec2i dmMapToIdx(dmMap this,cxVec2f pos)
{
    cxSize2f size = cxViewSize(this);
    cxFloat x = pos.x + size.w / 2.0f;
    cxFloat y = pos.y + size.h / 2.0f;
    return cxVec2iv(x/this->gridSize.w, y/this->gridSize.h);
}

cxVec2f dmMapToPos(dmMap this,cxVec2i idx)
{
    cxSize2f size = cxViewSize(this);
    cxFloat x = idx.x * this->gridSize.w - size.w / 2.0f;
    cxFloat y = idx.y * this->gridSize.h - size.h / 2.0f;
    return cxVec2fv(x + this->gridSize.w/2.0f, y + this->gridSize.h/2.0f);
}

static cxBool dmMapTouch(cxAny pview,cxTouch *touch)
{
    dmMap this = pview;
    cxVec2f pos;
    if(!cxViewHitTest(this, touch->current, &pos)){
        return false;
    }
    if(touch->type == cxTouchTypeDown){
        cxVec2i idx = dmMapToIdx(this, pos);
        this->values[idx.x][idx.y] = 1;
        if(!dmMapFindPath(this, cxVec2iv(0, 0), cxVec2iv(9, 9))){
            this->values[idx.x][idx.y] = 0;
            return false;
        }
        
        dmSprite sp = dmSpriteCreate(this, idx);
        cxViewAppend(this, sp);
        cxViewAppend(this, sp);
        cxObjectSetTag(sp, 1);
        cxSpriteSetTextureURL(sp, "item.xml?blue.png", false);
        
        CX_LIST_FOREACH(this->super.subViews, ele){
            cxInt tag = cxObjectTag(ele->any);
            if(tag != 1){
                cxViewRemoved(ele->any);
            }
        }
        
        for(cxInt i=0; i < this->path.number; i++){
            cxVec2i idx = this->path.points[i];
            dmSprite sp = dmSpriteCreate(this, idx);
            cxViewAppend(this, sp);
        }
    }
    return false;
}

cxBool dmMapFindPath(dmMap this,cxVec2i start,cxVec2i stop)
{
    cxBool rv = false;
    ASPath path = ASPathCreate(&PathNodeSource, this, &start, &stop);
    this->path.number = 0;
    for (int i=0; i<ASPathGetCount(path); i++) {
        cxVec2i *pathNode = ASPathGetNode(path, i);
        this->path.points[this->path.number++] = *pathNode;
    }
    rv = (ASPathGetCount(path) > 0);
    ASPathDestroy(path);
    return rv;
}

CX_OBJECT_INIT(dmMap, cxView)
{
    CX_METHOD_OVERRIDE(this->super.Touch, dmMapTouch);
    cxSize2f size = cxEngineInstance()->winsize;
    cxFloat vw = size.w - 20;
    this->gridSize.w = vw / DM_MAP_WIDTH;
    this->gridSize.h = this->gridSize.w;
    cxViewSetSize(this, cxSize2fv(vw, vw));
}
CX_OBJECT_FREE(dmMap, cxView)
{
    
}
CX_OBJECT_TERM(dmMap, cxView)










