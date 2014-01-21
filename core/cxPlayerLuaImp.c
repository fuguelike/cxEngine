//
//  cxPlayerLuaImp.c
//  cxEngineIOS
//
//  Created by xuhua on 12/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxPlayer.h"

static cxInt cxPlayLuaEffect(lua_State *L)
{
    cxConstChars file = luaL_checkstring(L, 1);
    cxBool loop = lua_toboolean(L, 2);
    cxAny any = cxPlayEffect(file, loop);
    CX_LUA_PUSH_OBJECT(any);
    return 1;
}

static cxInt cxPlayLuaMusic(lua_State *L)
{
    cxConstChars file = luaL_checkstring(L, 1);
    cxBool loop = lua_toboolean(L, 2);
    cxPlayMusic(file, loop);
    return 0;
}

CX_LUA_METHOD_BEG(cxPlayer)
    {"PlayEffect",cxPlayLuaEffect},
    {"PlayMusic",cxPlayLuaMusic},
CX_LUA_METHOD_END(cxPlayer)

void cxPlayerTypeInit()
{
    CX_LUA_LOAD_TYPE(cxPlayer);
}