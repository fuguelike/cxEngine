//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include "Map.h"
#include "Node.h"
#include "Scene.h"
#include "Button.h"

void cxEngineType(cxEngine engine)
{
    CX_TYPE_REG(Map);
    CX_TYPE_REG(Node);
    CX_TYPE_REG(Scene);
    CX_TYPE_REG(Button);
}

void cxEngineInit(cxEngine engine)
{
//    engine->isShowBorder = false;
}

void cxEngineMain(cxEngine engine)
{
    cxLoader loader = cxLoaderCreate("main.json");
    Scene scene = CX_CAST(Scene, loader->object);
    CX_ASSERT(scene != NULL, "get main scene failed");
    scene->map = cxLoaderGet(loader, "map");
    MapInit(scene->map,NULL);
    cxWindowPushView(scene);
}

void cxEngineFree(cxEngine engine)
{
    
}
