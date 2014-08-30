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
}

static cxBool touch(cxAny pview,cxTouch *touch)
{
    cxVec2f pos;
    if(cxViewHitTest(pview, touch->current, &pos)){
        
        pos = cxViewPointToWindowPoint(pview, cxVec2fv(0, 0));
        CX_LOGGER("---- %f %f",pos.x,pos.y);
    }
    return false;
}

void cxEngineMain(cxEngine engine)
{
    cxViewLoader loader = cxViewLoaderCreate("cxView.json");
    cxWindowPushView(loader->view);
    cxView v = loader->view;
    CX_METHOD_SET(v->Touch, touch);
}

void cxEngineFree(cxEngine engine)
{
    
}
