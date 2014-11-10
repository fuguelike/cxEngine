//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include <views/cxSprite.h>

#include <net/cxTCP.h>

CX_METHOD_DEF(cxEngine, Update, void)
{
    
}

void cxEngineType(cxType this)
{
    CX_METHOD(cxEngine, Update);
}
void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesSize(cxSize2fv(2048, 1536));
    cxTCP tcp = CX_CREATE(cxTCP);
//    cxTCPClose(tcp);
    CX_RETAIN(tcp);
    cxTCPConnect(tcp,UTF8("192.168.10.144"), 8082);
}

void cxEngineMain(cxEngine engine)
{
    cxLoader loader = cxLoaderCreate("cxAtlas.json");
    cxWindowPushView(loader->Root);
}

void cxEngineFree(cxEngine engine)
{
    
}
