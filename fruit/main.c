//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <views/cxButton.h>
#include <core/cxEngine.h>
#include <core/cxViewRoot.h>
#include "fcMap.h"


static cxAny fcMakeView(cxConstChars temp, xmlTextReaderPtr reader)
{
    if(ELEMENT_IS_TYPE(fcMap)){
        return CX_CREATE(fcMap);
    }
    return NULL;
}

static cxInt fcMapSetEditMode(lua_State *L)
{
    CX_LUA_DEF_THIS(cxButton);
    fcMap map = cxViewRootGet(this, "map");
    fcMapSetMode(map, fcMapModeEdit);
    return 0;
}

static cxInt fcMapSetFireMode(lua_State *L)
{
    CX_LUA_DEF_THIS(cxButton);
    fcMap map = cxViewRootGet(this, "map");
    fcMapSetMode(map, fcMapModeFire);
    return 0;
}

void cxEngineInit(cxEngine engine)
{
    cxEngineRegisteFunc(fcMapSetEditMode);
    cxEngineRegisteFunc(fcMapSetFireMode);
    CX_METHOD_OVERRIDE(engine->MakeView, fcMakeView);
    __fcMapTypeInit();
}

void cxEngineMain(cxEngine engine)
{
    cxWindowPushXML("main.xml");
}

void cxEngineFree(cxEngine engine)
{
    
}
