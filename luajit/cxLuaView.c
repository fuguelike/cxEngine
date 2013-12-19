//
//  cxLuaView.c
//  cxEngine
//
//  Created by xuhua on 12/19/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxLuaLoader.h"
#include "cxLuaView.h"

const luaL_reg cxViewFuns [] = {

    {NULL, NULL}
};

void cxLuaViewLoad(cxAny loader)
{
    cxLuaLoader this = loader;
    luaL_register(this->L, "cxView", cxViewFuns);
}