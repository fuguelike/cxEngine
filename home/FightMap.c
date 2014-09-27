//
//  FightMap.c
//  cxCore
//
//  Created by xuhua on 9/27/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "FightMap.h"
#include "Map.h"
#include "Node.h"
#include "Move.h"
#include "Bullet.h"
#include <types/Attack.h>
#include <types/Defence.h>
#include <types/Wall.h>

static cxBool MapFightTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, Map);
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    cxVec2f cpos;
    if(!cxViewHitTest(pview, item->position, &cpos)){
        return false;
    }
    if(item->type == cxTouchTypeDown){
        this->isSelectUnit = true;
        //        cxVec2f idx = MapPosToPos(this, cpos);
        //        CX_LOGGER("%f %f",idx.x,idx.y);
        return false;
    }
    if(item->type == cxTouchTypeUp && item->movement < 30){
        cxVec2i idx = MapPosToIndex(this, cpos);
        cxAny item = MapItem(this, idx);
        if(item != NULL){
            return false;
        }
        CX_LOGGER("touch point:%d %d",idx.x,idx.y);
        //test
        if(this->tag == 1){
            Attack node = AttackCreate(this, idx);
            NodeSetLife(node, 200);
            NodeSetPower(node, 50);
            MapAppendNode(node);
        }else if(this->tag == 2){
            Defence node = DefenceCreate(this, idx);
            NodeSetPower(node, 40);
            NodeSetLife(node, 200);
            MapAppendNode(node);
        }else if(this->tag == 3){
            if(MapItem(this, idx) != NULL){
                return false;
            }
            Wall node = WallCreate(this, idx);
            NodeSetLife(node, 100);
            MapAppendNode(node);
        }else if(this->tag == 4){
            
        }
        return true;
    }
    return false;
}

static void mapSubType(cxAny dst,cxAny src)
{
    CX_ASSERT_THIS(dst, Map);
    this->tag = cxViewTag(src);
    if(this->tag == 5){
        for(cxInt x = 0; x < MAP_COL; x++){
            for (cxInt y = 4; y < 30; y ++) {
                Wall node = WallCreate(this, cxVec2iv(x, y));
                cxViewSetColor(node, cxBLACK);
                NodeSetLife(node, 100);
                MapAppendNode(node);
            }
        }
    }else if(this->tag == 6){
        MapRemoveNodes(this);
    }
}

CX_OBJECT_TYPE(FightMap, Map)
{
    
}
CX_OBJECT_INIT(FightMap, Map)
{
    MapSetMode(this, MapModeFight);
    cxMessageAppend(this, mapSubType, "selectSubType");
    CX_METHOD_SET(CX_TYPE(cxView, this)->Touch, MapFightTouch);
}
CX_OBJECT_FREE(FightMap, Map)
{
    cxMessageRemove(this);
}
CX_OBJECT_TERM(FightMap, Map)

cxBool FightMapInit(cxAny pmap)
{
    CX_ASSERT_TYPE(pmap, FightMap);
    if(!MapInit(pmap)){
        return false;
    }
    return true;
}