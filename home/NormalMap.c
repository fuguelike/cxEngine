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

static cxBool NormalMapTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, NormalMap);
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    cxVec2f cpos;
    if(!cxViewHitTest(this, item->position, &cpos)){
        return false;
    }
    cxVec2f index = MapPosToFloat(this, cpos);
    if(item->type == cxTouchTypeUp){
        this->unode = MapHitNode(this, index, NodeTypeAll);
        if(this->currNode != NULL){
            NodeUpdateInitIndex(this->currNode);
        }
        if(item->isTap && this->dnode == this->unode){
            if(this->currNode != NULL && !NodeUpdateInitIndex(this->currNode)){
                NodeResetIndex(this->currNode);
            }
            if(this->currNode != NULL){
                //process prev selected node
            }
            this->currNode = this->dnode;
            if(this->currNode != NULL){
                cxViewBringFront(this->currNode);
                //node 被选中
                CX_LOGGER("select node");
            }
            return false;
        }
    }
    if(item->type == cxTouchTypeDown){
        this->prevIdx = MapPosToFloat(this, cpos);
        this->dnode = MapHitNode(this, index, NodeTypeAll);
        return false;
    }
    if(item->type == cxTouchTypeMove && this->dnode != NULL && this->dnode == this->currNode){
        cxVec2f currIdx = MapPosToFloat(this, cpos);
        cxVec2f delta;
        kmVec2Subtract(&delta, &currIdx, &this->prevIdx);
        
        cxVec2i idx = cxVec2iv(delta.x, delta.y);
        cxBool setPos = false;
        cxVec2f nidx = NodeGetIndex(this->currNode);
        if(idx.x != 0){
            setPos = true;
            nidx.x += idx.x;
            this->prevIdx.x = currIdx.x - fmodf(delta.x, 1.0f);
        }
        if(idx.y != 0){
            setPos = true;
            nidx.y += idx.y;
            this->prevIdx.y = currIdx.y - fmodf(delta.y, 1.0f);;
        }
        if(setPos){
            cxVec2f npos = MapIndexToPos(this, nidx);
            cxViewSetPosition(this->currNode, npos);
            cxVec2i newIdx = NodeIndexToInitIndex(this->currNode,nidx);
            //检测位置是否合法
            if(MapIsFillNode(this, newIdx, this->currNode)){
                //合法的位置
                cxViewSetColor(this->currNode, cxGREEN);
            }else{
                //错误的位置
                cxViewSetColor(this->currNode, cxRED);
            }
        }
        return true;
    }
    return false;
}

CX_OBJECT_TYPE(NormalMap, Map)
{
    
}
CX_OBJECT_INIT(NormalMap, Map)
{
    MapSetMode(this, MapModeNormal);
    CX_SET(cxView, this, Touch, NormalMapTouch);
    CX_SET(Map, this, NodeMove, NormalNodeMove);
}
CX_OBJECT_FREE(NormalMap, Map)
{
    
}
CX_OBJECT_TERM(NormalMap, Map)

cxBool NormalMapInit(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, NormalMap);
    if(!MapInit(pmap)){
        return false;
    }
    cxAny node1 = WallCreate(this, cxVec2iv(8, 8));
    MapAppendNode(node1);
    
    cxAny node2 = DefenceCreate(this, cxVec2iv(20, 20));
    MapAppendNode(node2);
    return true;
}

CX_OBJECT_TYPE(NormalScene, cxScroll)
{
    
}
CX_OBJECT_INIT(NormalScene, cxScroll)
{
    
}
CX_OBJECT_FREE(NormalScene, cxScroll)
{
    
}
CX_OBJECT_TERM(NormalScene, cxScroll)

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