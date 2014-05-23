//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>

void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesignSize(cxSize2fv(640, 960));
}

void cxEngineMain(cxEngine engine)
{
    cxViewLoader loader = cxViewLoaderCreate("cxScroll.json");
    cxWindowPushView(loader->root);
}

void cxEngineFree(cxEngine engine)
{
    
}
