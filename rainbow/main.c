//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include "rbMap.h"

static cxAny rbMakeView(cxConstChars temp, xmlTextReaderPtr reader)
{
    if(ELEMENT_IS_TYPE(rbMap)){
        return CX_CREATE(rbMap);
    }
    return NULL;
}

void cxEngineInit(cxEngine engine)
{
    CX_METHOD_OVERRIDE(engine->MakeView, rbMakeView);
    
}

void cxEngineMain(cxEngine engine)
{
    cxWindowPushXML("main.xml");
}

void cxEngineFree(cxEngine engine)
{
    
}
