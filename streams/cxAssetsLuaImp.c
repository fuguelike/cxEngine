//
//  cxAssetsLuaImp.c
//  cxEngineIOS
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxAssetsStream.h"

const luaL_Reg cxAssetsStreamInstanceMethods[] = {
    CX_LUA_SUPER(cxStream)
};

static cxInt cxAssetsStreamLuaCreate(lua_State *L)
{
    cxStream stream = cxAssetsStreamCreate(luaL_checkstring(L, 1));
    CX_LUA_PUSH_OBJECT(stream);
    return 1;
}

const luaL_Reg cxAssetsStreamTypeMethods[] = {
    {"create",cxAssetsStreamLuaCreate},
    {NULL,NULL}
};

void cxAssetsStreamTypeInit()
{
    CX_LUA_LOAD_TYPE(cxAssetsStream);
}