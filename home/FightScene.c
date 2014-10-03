//
//  Scene.c
//  Home
//
//  Created by xuhua on 8/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "FightScene.h"
#include "WarScene.h"
#include "FightMap.h"
#include "Button.h"

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
    cxLoader loader = cxLoaderCreate("main.json");
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
    
    FightMapInit(scene->fightMap);
    
    return scene;
}

