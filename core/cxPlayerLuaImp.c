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

static cxInt cxStopLuaEffect(lua_State *L)
{
    CX_LUA_DEF_THIS(cxAny);
    cxStopEffect(this);
    return 0;
}

static cxInt cxPauseLuaEffect(lua_State *L)
{
    CX_LUA_DEF_THIS(cxAny);
    cxPauseEffect(this);
    return 0;
}

static cxInt cxResumeLuaEffect(lua_State *L)
{
    CX_LUA_DEF_THIS(cxAny);
    cxResumeEffect(this);
    return 0;
}

static cxInt cxPlayLuaMusic(lua_State *L)
{
    cxConstChars file = luaL_checkstring(L, 1);
    cxBool loop = lua_toboolean(L, 2);
    cxPlayMusic(file, loop);
    return 0;
}

static cxInt cxStopLuaMusic(lua_State *L)
{
    cxStopMusic();
    return 0;
}

static cxInt cxPauseLuaMusic(lua_State *L)
{
    cxPauseMusic();
    return 0;
}

static cxInt cxResumeLuaMusic(lua_State *L)
{
    cxResumeMusic();
    return 0;
}

CX_LUA_METHOD_BEG(cxPlayer)
    {"PlayEffect",cxPlayLuaEffect},
    {"StopEffect",cxStopLuaEffect},
    {"PauseEffect",cxPauseLuaEffect},
    {"ResumeEffect",cxResumeLuaEffect},
    {"PlayMusic",cxPlayLuaMusic},
    {"StopMusic",cxStopLuaMusic},
    {"PauseMusic",cxPauseLuaMusic},
    {"ResumeMusic",cxResumeLuaMusic},
CX_LUA_METHOD_END(cxPlayer)

void __cxPlayerTypeInit()
{
    CX_LUA_LOAD_TYPE(cxPlayer);
}


