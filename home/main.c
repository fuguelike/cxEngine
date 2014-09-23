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
#include <types/Soldier.h>
#include <types/Turret.h>

void cxEngineType(cxEngine engine)
{
    CX_TYPE_REG(Map);
    CX_TYPE_REG(Node);
    CX_TYPE_REG(Scene);
    CX_TYPE_REG(Button);
    CX_TYPE_REG(Range);
    CX_TYPE_REG(Move);
    
    CX_TYPE_REG(Soldier);
    CX_TYPE_REG(Turret);
}

static void selectButton(cxAny pview)
{
    cxMessagePost("selectSubType", pview);
}

void cxEngineInit(cxEngine engine)
{
    engine->isShowBorder = true;
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
    Scene scene = CX_TYPE_CAST(Scene, loader->object);
    scene->map = cxLoaderGet(loader, "map");
    CX_ASSERT(scene->map != NULL, "map null");
    
    //test code
    Button btnSelectTurret = cxLoaderGet(loader, "btnSelectTurret");
    CX_EVENT_APPEND(btnSelectTurret->onTap, selectButton);
    
    Button btnSelectSoldier = cxLoaderGet(loader, "btnSelectSoldier");
    CX_EVENT_APPEND(btnSelectSoldier->onTap, selectButton);
    
    Button btnSelectArcher = cxLoaderGet(loader, "btnSelectArcher");
    CX_EVENT_APPEND(btnSelectArcher->onTap, selectButton);
    
    
    MapInit(scene->map,NULL);
    cxWindowPushView(scene);
}

void cxEngineFree(cxEngine engine)
{
    
}
