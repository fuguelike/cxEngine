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
//    cxFloat w = 225/2;
//    cxFloat h = 170/2;
//    cxFloat x = atan2f(w, h);
//    cxFloat y = atan2f(h, w);
//    x = kmRadiansToDegrees(x);
//    y = kmRadiansToDegrees(y);
//    CX_LOGGER("%f",x*2 + y*2);
}

void cxEngineMain(cxEngine engine)
{
    cxViewLoader loader = cxViewLoaderCreate("cxScroll.json");
    cxWindowPushView(loader->view);
}

void cxEngineFree(cxEngine engine)
{
    
}
