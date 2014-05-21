//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include "wmMap.h"
#include "wmMine.h"

void cxEngineInit(cxEngine engine)
{
    CX_REGISTER_TYPE(wmMap,     cxView);
    CX_REGISTER_TYPE(wmMine,    cxSprite);
}

void cxEngineMain(cxEngine engine)
{
    cxController controller = cxControllerCreate("main.json");
    cxWindowPushView(controller->root);
}

void cxEngineFree(cxEngine engine)
{
    
}
