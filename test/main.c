//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>

void cxEngineType(cxEngine engine)
{
    
}

void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesignSize(cxSize2fv(640, 960));
}

static void exitact(cxAny sender)
{
    cxViewRemove(cxActionGetView(sender));
}

void cxEngineMain(cxEngine engine)
{
    cxLoader loader = cxLoaderCreate("cxSprites.json");
    cxAction a = cxLoaderGet(loader, "animate");
    CX_ADD(cxAction, a, onExit, exitact);
    cxWindowPushView(loader->Root);
}

void cxEngineFree(cxEngine engine)
{
    
}
