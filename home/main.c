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
#include "Range.h"

void cxEngineType(cxEngine engine)
{
    CX_TYPE_REG(Map);
    CX_TYPE_REG(Node);
    CX_TYPE_REG(Scene);
    CX_TYPE_REG(Button);
    CX_TYPE_REG(Range);
}

void cxEngineInit(cxEngine engine)
{
//    engine->isShowBorder = false;
}

void cxEngineMain(cxEngine engine)
{
    GlobalInit(engine);
    
    cxLoader loader = cxLoaderCreate("main.json");
    Scene scene = CX_TYPE_CAST(Scene, loader->object);
    CX_ASSERT(scene != NULL, "get main scene failed");
    scene->map = cxLoaderGet(loader, "map");
    CX_ASSERT(scene->map != NULL, "map null");
    MapInit(scene->map,NULL);
    cxWindowPushView(scene);
    
//    Range r = CX_CREATE(Range);
//    RangeSetMax(r, 11);
//    RangeSetMin(r, 4);
//    cxViewAppend(scene->map, r);
}

void cxEngineFree(cxEngine engine)
{
    
}
