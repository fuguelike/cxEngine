//
//  cxBase.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "cxBase.h"

void cxObjectLuaEventFunc(cxEvent *event)
{
    cxEngine engine = cxEngineInstance();
    lua_State *L = engine->L;
    cxInt ref = cxEventArgToRef(event->args);
    CX_ASSERT(ref > 0, "args ref error");
    lua_getref(L, ref);
    CX_LUA_PUSH_OBJECT(event->sender);
    lua_call(L, 1, 0);
}


