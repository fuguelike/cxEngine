//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include "dmMap.h"

void cxEngineInit(cxEngine engine)
{
    
}

void cxEngineMain(cxEngine engine)
{
    dmMap map = CX_CREATE(dmMap);
    cxWindowPushView(map, NULL);
}

void cxEngineFree(cxEngine engine)
{
    
}
