//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include "fcMap.h"

void cxEngineInit(cxEngine engine)
{
    
}

void cxEngineMain(cxEngine engine)
{
    fcMap map = CX_CREATE(fcMap);
    cxWindowPushView(map, NULL);
}

void cxEngineFree(cxEngine engine)
{
    
}
