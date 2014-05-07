//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>

void cxEngineInit(cxEngine engine)
{
    
}

void cxEngineMain(cxEngine engine)
{
    cxAny view = cxViewLoad("ui.json", NULL);
    cxWindowPushView(view);
}

void cxEngineFree(cxEngine engine)
{
    
}
