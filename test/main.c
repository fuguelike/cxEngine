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
    cxEngineSetDesSize(cxSize2fv(2048, 1536));
}

void cxEngineMain(cxEngine engine)
{
    cxLoader loader = cxLoaderCreate("cxLabelTTF.json");
    cxWindowPushView(loader->Root);
}

void cxEngineFree(cxEngine engine)
{
    
}
