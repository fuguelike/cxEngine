//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include "rbMap.h"

void cxEngineInit(cxEngine engine)
{
    
}

#include <views/cxSprite.h>
void cxEngineMain(cxEngine engine)
{
    rbMap map = CX_CREATE(rbMap);
    cxViewSetAutoResizeBox(map, cxBox4fv(0, 0, 0, 9));
    cxViewSetAutoResizeMask(map, cxViewAutoResizeBottom);
    cxWindowPushView(map);
}

void cxEngineFree(cxEngine engine)
{
    
}
