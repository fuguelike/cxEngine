//
//  dmMap.c
//  dome
//
//  Created by xuhua on 3/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "dmMap.h"
#include <algorithm/cxAStar.h>

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
    //left-up
    cxVec2i leftUp = cxVec2iv(pathNode->x + 1, pathNode->y + 1);
    cxInt leftUpValue = dmMapValue(map, leftUp);
    if(leftUpValue == 0){
        ASNeighborListAdd(neighbors, &leftUp, 1);
    }
    //left-down
    cxVec2i leftDown = cxVec2iv(pathNode->x + 1, pathNode->y - 1);
    cxInt leftDownValue = dmMapValue(map, leftDown);
    if(leftDownValue == 0){
        ASNeighborListAdd(neighbors, &leftDown, 1);
    }
    //right-up
    cxVec2i rightUp = cxVec2iv(pathNode->x - 1, pathNode->y + 1);
    cxInt rightUpValue = dmMapValue(map, rightUp);
    if(rightUpValue == 0){
        ASNeighborListAdd(neighbors, &rightUp, 1);
    }
    //right-down
    cxVec2i rightDown = cxVec2iv(pathNode->x - 1, pathNode->y - 1);
    cxInt rightDownValue = dmMapValue(map, rightDown);
    if(rightDownValue == 0){
        ASNeighborListAdd(neighbors, &rightDown, 1);
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
        cxVec2f pos = dmMapToPos(this, idx);
        cxView v = CX_CREATE(cxView);
        cxViewSetSize(v, this->gridSize);
        cxViewSetPos(v, pos);
        cxViewAppend(this, v);
        CX_LOGGER("x=%d y=%d",idx.x,idx.y);
        this->values[idx.x][idx.y] = 1;
        cxFloat cost = dmMapFindPath(this, cxVec2iv(0, 0), cxVec2iv(9, 9));
        CX_LOGGER("cost=%f %d",cost,this->path.number);
    }
    return false;
}

cxFloat dmMapFindPath(dmMap this,cxVec2i start,cxVec2i stop)
{
    ASPath path = ASPathCreate(&PathNodeSource, this, &start, &stop);
    this->path.number = 0;
    for (int i=0; i<ASPathGetCount(path); i++) {
        cxVec2i *pathNode = ASPathGetNode(path, i);
        this->path.points[this->path.number++] = *pathNode;
    }
    cxFloat cost = ASPathGetCost(path);
    ASPathDestroy(path);
    return cost;
}

CX_OBJECT_INIT(dmMap, cxView)
{
    CX_METHOD_OVERRIDE(this->super.Touch, dmMapTouch);
    cxSize2f size = cxEngineInstance()->winsize;
    this->gridSize.w = (size.w - 10) / DM_MAP_WIDTH;
    this->gridSize.h = this->gridSize.w;
    cxViewSetSize(this, cxSize2fv(size.w - 10,size.w - 10));
}
CX_OBJECT_FREE(dmMap, cxView)
{
    
}
CX_OBJECT_TERM(dmMap, cxView)










