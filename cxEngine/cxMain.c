//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxDB.h>

void cxEngineInit(cxEngine engine)
{
    cxEngineTypes("items.xml");
    
    cxEngineLuaRunChars("require('main')");
    
    cxEngineLuaRunChars("init()");
    
    cxAny db = cxEngineDB("items.xml?db2");
    cxDBSet(db, UTF8("111"), UTF8("%d",100));
}

void cxEngineMain(cxEngine engine)
{
    cxEngineLuaRunChars("main()");
}

void cxEngineFree(cxEngine engine)
{
    cxEngineLuaRunChars("free()");
}
















