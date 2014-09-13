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

#include <streams/cxMMapStream.h>

void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesignSize(cxSize2fv(640, 960));
    
    cxStream s = cxMMapStreamCreate("red.png");
    cxString data = cxStreamAllBytes(s);
    if(data != NULL){
        CX_LOGGER("test mmap data %d",cxStringLength(data));
    }else{
        CX_ERROR("test mmap error");
    }
}

void cxEngineMain(cxEngine engine)
{
    cxLoader loader = cxLoaderCreate("cxLayer.json");
    cxWindowPushView(loader->object);
}

void cxEngineFree(cxEngine engine)
{
    
}
