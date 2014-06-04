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

#include <socket/cxHttp.h>

void cxEngineInit(cxEngine engine)
{
    cxHttp http = cxHttpPost("http://127.0.0.1:9090", UTF8("1234"), true);
    cxEngineSetDesignSize(cxSize2fv(640, 960));
}

void cxEngineMain(cxEngine engine)
{

}

void cxEngineFree(cxEngine engine)
{
    
}
