//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxView.h>
#include <actions/cxRotate.h>

void cxEngineInit(cxEngine engine)
{
    
}

void cxEngineMain(cxEngine engine)
{
    cxView v = CX_CREATE(cxView);
    cxViewSetSize(v, cxSize2fv(200, 200));
    cxWindowPushView(v);
}

void cxEngineFree(cxEngine engine)
{
    
}
