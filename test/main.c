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

#include <engine/cxSpatial.h>

void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesignSize(cxSize2fv(640, 960));
}

void cxSpatialEach3(cxAny ps,cxAny pview,cxAny data)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    cxView v = CX_TYPE_CAST(cxView, pview);
}

void cxEngineMain(cxEngine engine)
{
    
    cxSpatial idx = CX_CREATE(cxSpatial);
    
    {
        cxView v = CX_CREATE(cxView);
        cxViewSetPos(v, cxVec2fv(10, 10));
        cxViewSetSize(v, cxSize2fv(20, 20));
        cxViewSetTag(v, 1);
        cxViewAppend(engine->window, v);
        cxSpatialInsert(idx, v);
    }
    
    {
        cxView v = CX_CREATE(cxView);
        cxViewSetPos(v, cxVec2fv(20, 20));
        cxViewSetSize(v, cxSize2fv(20, 20));
        cxViewSetTag(v, 2);
        cxViewAppend(engine->window, v);
        cxSpatialInsert(idx, v);
    }
    
    {
        cxView v = CX_CREATE(cxView);
        cxViewSetPos(v, cxVec2fv(30, 30));
        cxViewSetSize(v, cxSize2fv(20, 20));
        cxViewSetTag(v, 3);
        cxViewAppend(engine->window, v);
        cxSpatialInsert(idx, v);
    }
    
    {
        cxView v = CX_CREATE(cxView);
        cxViewSetPos(v, cxVec2fv(-40, -40));
        cxViewSetSize(v, cxSize2fv(20, 20));
        cxViewSetTag(v, 4);
        cxViewAppend(engine->window, v);
        cxSpatialInsert(idx, v);
    }
    cxSpatialEach(idx, cxSpatialEach3, (cxAny)1);
    cxSpatialNearestInfo ret = cxSpatialNearest(idx, cxVec2fv(0, 0), 30, NULL);
    CX_LOGGER("%d",cxViewTag(ret.view));


//    cxLoader loader = cxLoaderCreate("cxLabelBMP.json");
//    cxWindowPushView(loader->object);
}

void cxEngineFree(cxEngine engine)
{
    
}
