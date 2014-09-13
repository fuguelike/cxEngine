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

#include <views/cxSpine.h>

void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesignSize(cxSize2fv(640, 960));
    
}

void cxEngineMain(cxEngine engine)
{
    cxSpine sp = CX_CREATE(cxSpine);
    cxViewSetSize(sp, cxSize2fv(500, 500));
    //    cxLoader loader = cxLoaderCreate("cxLayer.json");
    cxWindowPushView(sp);
}

void cxEngineFree(cxEngine engine)
{
    
}
