//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxJson.h>

void cxEngineInit(cxEngine engine)
{
    cxJson json = cxJsonCreate(UTF8("[1,3,4,5,6]"));
    cxInt a = cxJsonInt(json, "[1]", 0);
    cxInt b = 0;
}

void cxEngineMain(cxEngine engine)
{
   
}

void cxEngineFree(cxEngine engine)
{
    
}
