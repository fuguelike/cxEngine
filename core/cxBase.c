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
    cxInt ref = cxEventArgToRef(event->args);
    CX_ASSERT(ref > 0, "args ref error");
    lua_getref(gL, ref);
    lua_pushlightuserdata(gL, event->sender);
    cxInt an = cxEventArgPush(event->args) ? 2 : 1;
    lua_call(gL, an, 0);
}


