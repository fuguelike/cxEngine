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
    cxController controller = CX_ALLOC(cxController);
    cxControllerInitWithFile(controller, "ui.json");
    cxAny object = cxControllerGet(controller, "v0");
    if(CX_INSTANCE_OF(object, cxView)){
        cxWindowPushView(object);
    }
    CX_RELEASE(controller);
}

void cxEngineFree(cxEngine engine)
{
    
}
