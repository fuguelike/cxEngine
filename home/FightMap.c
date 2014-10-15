//
//  FightMap.c
//  cxCore
//
//  Created by xuhua on 9/27/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "Button.h"
#include "FightMap.h"
#include "Map.h"
#include "Node.h"
#include "Move.h"
#include "Bullet.h"
#include "WarMap.h"
#include <types/Attack.h>
#include <types/Defence.h>
#include <types/Wall.h>
#include <types/Longer.h>
#include <types/Flyable.h>
#include <actions/cxMove.h>

static cxBool FightMapTouch(cxAny pview,const cxTouchItems *points)
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
        return false;
    }
    if(item->type == cxTouchTypeUp && item->isTap){
        cxVec2i idx = MapPosToIndex(this, cpos);
        cxAny item = MapItem(this, idx);
        if(item != NULL){
            return false;
        }
        CX_LOGGER("touch point:%d %d",idx.x,idx.y);
        //test
        if(this->tag == 1){
            Attack node = AttackCreate(this, idx);
            NodeSetLife(node, cxRange2iv(2000, 2000));
            NodeSetPower(node, 10);
            MapAppendNode(node);
        }else if(this->tag == 2){
            Defence node = DefenceCreate(this, idx);
            NodeSetPower(node, 10);
            NodeSetLife(node, cxRange2iv(2000, 2000));
            MapAppendNode(node);
        }else if(this->tag == 3){
            if(MapItem(this, idx) != NULL){
                return false;
            }
            Wall node = WallCreate(this, idx);
            NodeSetLife(node, cxRange2iv(100, 100));
            MapAppendNode(node);
            cxViewSetColor(node, cxColor3fv(1.3f, 1.3f, 1.3f));
            cxViewSetAlpha(node, 1.3f);
        }else if(this->tag == 4){
            Longer node = LongerCreate(this, idx);
            NodeSetLife(node, cxRange2iv(200, 200));
            NodeSetPower(node, 10);
            MapAppendNode(node);
        }else if(this->tag == 7){
            Flyable node = FlyableCreate(this, idx);
            NodeSetLife(node, cxRange2iv(200, 200));
            NodeSetPower(node, 10);
            MapAppendNode(node);
        }
        return true;
    }
    return false;
}

static void mapSubType(cxAny dst,cxAny src)
{
    CX_ASSERT_THIS(dst, Map);
    this->tag = cxViewGetTag(src);
    if(this->tag == 5){
        
        for(cxInt x = 0; x < MAP_COL; x++){
            for (cxInt y = 0; y < MAP_ROW; y ++) {
                if(!((x % 3) == 0)){
                    continue;
                }
                Wall node = WallCreate(this, cxVec2iv(x, y));
                cxViewSetColor(node, cxBLACK);
                NodeSetLife(node, cxRange2iv(200, 200));
                MapAppendNode(node);
            }
        }
        
        cxArray items = MapNearestItems(this, cxVec2fv(20.5f, 20.5f), cxRange2fv(0, 5), NodeCombinedMake(NodeTypeBlock, NodeSubTypeNone));
        
        CX_ARRAY_FOREACH(items, ele){
            Node node = cxArrayObject(ele);
            cxViewSetColor(node, cxRED);
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
    CX_SET(cxView, this, Touch, FightMapTouch);
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

static void selectButton(cxAny pview)
{
    cxMessagePost("selectSubType", pview);
}

static void btnToWarClick(cxAny sender)
{
    WarScene scene = WarSceneCreate();
    cxWindowPushView(scene);
}

CX_OBJECT_TYPE(FightScene, cxScroll)
{
    
}
CX_OBJECT_INIT(FightScene, cxScroll)
{
    
}
CX_OBJECT_FREE(FightScene, cxScroll)
{
    
}
CX_OBJECT_TERM(FightScene, cxScroll)

FightScene FightSceneCreate()
{
    cxLoader loader = cxLoaderCreate("fight.json");
    CX_ASSERT_VALUE(cxLoaderGetRoot(loader), FightScene, scene);
    scene->fightMap = cxLoaderGet(loader, "map");
    CX_ASSERT_TYPE(scene->fightMap, FightMap);
    
    //test code
    CX_LOADER_DEF(loader, Button, btnSelectTurret);
    CX_ADD(Button, btnSelectTurret, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnSelectSoldier);
    CX_ADD(Button, btnSelectSoldier, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnSelectArcher);
    CX_ADD(Button, btnSelectArcher, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnTest);
    CX_ADD(Button, btnTest, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnLonger);
    CX_ADD(Button, btnLonger, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnClear);
    CX_ADD(Button, btnClear, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnFlyable);
    CX_ADD(Button, btnFlyable, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnToWar);
    CX_ADD(Button, btnToWar, onTap, btnToWarClick);
    
    if(!FightMapInit(scene->fightMap)){
        CX_ERROR("init normal map error");
        return NULL;
    }
    return scene;
}
