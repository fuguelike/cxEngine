//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include <engine/cxGroup.h>
#include "Map.h"
#include "Node.h"
#include "Scene.h"
#include "Button.h"
#include "Range.h"
#include "Move.h"
#include "Bullet.h"
#include "FightMap.h"
#include <types/Attack.h>
#include <types/Defence.h>
#include <types/Wall.h>
#include <types/Longer.h>
#include <types/Flyable.h>
#include "WarMap.h"
#include "WarScene.h"
#include "NormalMap.h"

void cxEngineType(cxEngine engine)
{
    CX_TYPE_REG(Map);
    CX_TYPE_REG(Node);
    CX_TYPE_REG(Scene);
    CX_TYPE_REG(Button);
    CX_TYPE_REG(Range);
    CX_TYPE_REG(Move);
    
    CX_TYPE_REG(WarMap);
    CX_TYPE_REG(WarScene);
    
    CX_TYPE_REG(FightMap);
    CX_TYPE_REG(NormalMap);
    
    CX_TYPE_REG(Bullet);
    CX_TYPE_REG(Attack);
    CX_TYPE_REG(Defence);
    CX_TYPE_REG(Wall);
    CX_TYPE_REG(Flyable);
    CX_TYPE_REG(Longer);
}

static void selectButton(cxAny pview)
{
    cxMessagePost("selectSubType", pview);
}

void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesignSize(cxSize2fv(2048, 1536));
    engine->isShowBorder = true;
    //战斗用动画管理器
    cxGroup fight = CX_CREATE(cxGroup);
    cxGroupSetScale(fight, 1.0f);
    cxGroupSet(FIGHT_ACTION_GROUP, fight);
}
void cxEngineMain(cxEngine engine)
{
    //全局数据初始化
    GlobalInit(engine);
    
    cxLoader loader = cxLoaderCreate("main.json");
    CX_ASSERT_VALUE(cxLoaderGetRoot(loader), Scene, scene);
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
    
    FightMapInit(scene->fightMap);
    cxWindowPushView(scene);
}

void cxEngineFree(cxEngine engine)
{
    
}
