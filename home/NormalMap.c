//
//  NormalMap.c
//  cxCore
//
//  Created by xuhua on 10/2/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "NormalMap.h"
#include <types/Wall.h>
#include <algorithm/cxTile.h>
#include <types/Defence.h>

static void NormalNodeMove(cxAny pmap,cxAny node,cxVec2i from,cxVec2i to)
{
    CX_LOGGER("node %p from(%d,%d) to (%d,%d)",node,from.x,from.y,to.x,to.y);;
}

static cxBool NormalMapTouch(cxAny pview,const cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, NormalMap);
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    cxHitInfo hit = cxViewHitTest(this, item->position);
    if(!hit.hited){
        return false;
    }
    cxVec2f index = MapPosToFloat(this, hit.position);
    if(item->type == cxTouchTypeUp){
        this->unode = MapHitNode(this, index, NodeTypeAll);
        if(this->cnode != NULL){
            NodeUpdateInitIndex(this->cnode);
        }
        if(item->isTap && this->dnode == this->unode){
            if(this->cnode != NULL && !NodeUpdateInitIndex(this->cnode)){
                NodeResetIndex(this->cnode);
            }
            if(this->cnode != NULL){
                //process prev selected node
            }
            this->cnode = this->dnode;
            if(this->cnode != NULL){
                cxViewBringFront(this->cnode);
                //node 被选中
                CX_LOGGER("select node %p",this->cnode);
            }
        }
        return this->hasMove;
    }
    if(item->type == cxTouchTypeDown){
        this->startPos = MapPosToFloat(this, hit.position);
        this->dnode = MapHitNode(this, index, NodeTypeAll);
        this->hasMove = false;
        return false;
    }
    if(item->type == cxTouchTypeMove && this->dnode != NULL && this->dnode == this->cnode){
        cxVec2f currIdx = MapPosToFloat(this, hit.position);
        cxVec2f delta = cxVec2fSub(currIdx, this->startPos);
        cxVec2i idx = cxVec2fTo2i(delta);
        cxBool setPos = false;
        cxVec2f nidx = NodeGetIndex(this->cnode);
        if(idx.x != 0){
            setPos = true;
            nidx.x += idx.x;
            this->startPos.x = currIdx.x - fmodf(delta.x, 1.0f);
        }
        if(idx.y != 0){
            setPos = true;
            nidx.y += idx.y;
            this->startPos.y = currIdx.y - fmodf(delta.y, 1.0f);;
        }
        if(setPos){
            cxVec2f npos = MapIndexToPos(this, nidx);
            cxViewSetPosition(this->cnode, npos);
            cxVec2i newIdx = NodeIndexToInitIndex(this->cnode,nidx);
            cxBool isValid = MapIsFillNode(this, newIdx, this->cnode);
            cxViewSetColor(this->cnode, isValid?cxGREEN:cxRED);
            this->hasMove = true;
        }
        return true;
    }
    return false;
}

CX_TYPE(NormalMap, Map)
{
    
}
CX_INIT(NormalMap, Map)
{
    MapSetMode(this, MapModeNormal);
    CX_SET(cxView, this, Touch, NormalMapTouch);
    CX_SET(Map, this, NodeMove, NormalNodeMove);
}
CX_FREE(NormalMap, Map)
{
    
}
CX_TERM(NormalMap, Map)

cxBool NormalMapInit(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, NormalMap);
    if(!MapInit(pmap)){
        return false;
    }
    //Test
    cxAny node1 = WallCreate(this, cxVec2iv(8, 8));
    MapAppendNode(node1);
    
    cxAny node2 = DefenceCreate(this, cxVec2iv(20, 20));
    MapAppendNode(node2);
    return true;
}

CX_TYPE(NormalScene, cxScroll)
{
    
}
CX_INIT(NormalScene, cxScroll)
{
    
}
CX_FREE(NormalScene, cxScroll)
{
    
}
CX_TERM(NormalScene, cxScroll)

NormalScene NormalSceneCreate()
{
    cxLoader loader = cxLoaderCreate("normal.json");
    CX_ASSERT_VALUE(cxLoaderGetRoot(loader), NormalScene, scene);
    scene->normalMap = cxLoaderGet(loader, "map");
    CX_ASSERT_TYPE(scene->normalMap, NormalMap);
    if(!NormalMapInit(scene->normalMap)){
        CX_ERROR("init normal map error");
        return NULL;
    }
    return scene;
}

