//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include <engine/cxActionMgr.h>
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
#include <types/LongAttacker.h>

void cxEngineType(cxEngine engine)
{
    CX_TYPE_REG(Map);
    CX_TYPE_REG(Node);
    CX_TYPE_REG(Scene);
    CX_TYPE_REG(Button);
    CX_TYPE_REG(Range);
    CX_TYPE_REG(Move);
    
    CX_TYPE_REG(FightMap);
    
    CX_TYPE_REG(Bullet);
    CX_TYPE_REG(Attack);
    CX_TYPE_REG(Defence);
    CX_TYPE_REG(Wall);
    
    CX_TYPE_REG(LongAttacker);
}

static void selectButton(cxAny pview)
{
    cxMessagePost("selectSubType", pview);
}

void cxEngineInit(cxEngine engine)
{
#if !defined(NDEBUG)
    engine->isShowBorder = true;
#else
    engine->isShowBorder = false;
#endif
    
    //战斗用动画管理器
    cxActionMgr fight = CX_CREATE(cxActionMgr);
    fight->scale = 1.0f;
    cxActionMgrSet("fight", fight);
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
    CX_EVENT_APPEND(btnSelectTurret->onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnSelectSoldier);
    CX_EVENT_APPEND(btnSelectSoldier->onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnSelectArcher);
    CX_EVENT_APPEND(btnSelectArcher->onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnTest);
    CX_EVENT_APPEND(btnTest->onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnLonger);
    CX_EVENT_APPEND(btnLonger->onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnClear);
    CX_EVENT_APPEND(btnClear->onTap, selectButton);
    
    FightMapInit(scene->fightMap);
    cxWindowPushView(scene);
}

void cxEngineFree(cxEngine engine)
{
    
}
