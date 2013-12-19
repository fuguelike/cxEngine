//
//  cxLuaFuncs.c
//  cxEngine
//
//  Created by xuhua on 12/19/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxLuaLoader.h"
#include "cxLuaFuncs.h"

static cxString cxLuaPrintString(lua_State *L)
{
    int nargs = lua_gettop(L);
    cxString log = CX_CREATE(cxString);
    for (int i=1; i <= nargs; i++){
        if (lua_istable(L, i)){
            cxStringFormat(log, "table");
        }else if(lua_isnone(L, i)){
            cxStringFormat(log, "none");
        }else if(lua_isnil(L, i)){
            cxStringFormat(log, "nil");
        }else if(lua_isboolean(L, i)){
            cxStringFormat(log, (lua_toboolean(L, i) != 0) ? "false" : "true");
        }else if(lua_isfunction(L, i)){
            cxStringFormat(log, "function");
        }else if(lua_islightuserdata(L, i)){
            cxStringFormat(log, "lightuserdata");
        }else if(lua_isthread(L, i)){
            cxStringFormat(log, "thread");
        }else if(lua_isstring(L, i)){
            cxStringFormat(log, "%s",lua_tostring(L, i));
        }else if(lua_isnumber(L, i)){
            cxStringFormat(log, "%f",lua_tonumber(L, i));
        }else{
            cxStringFormat(log, "%s",lua_typename(L, i));
        }
    }
    return log;
}

static cxInt cxLuaLogger(lua_State *L)
{
    cxString log = cxLuaPrintString(L);
    CX_LOGGER("%s",cxStringBody(log));
    CX_UNUSED_PARAM(log);
    return 0;
}

static cxInt cxLuaError(lua_State *L)
{
    cxString log = cxLuaPrintString(L);
    CX_ERROR("%s",cxStringBody(log));
    return 0;
}

static cxInt cxLuaWarn(lua_State *L)
{
    cxString log = cxLuaPrintString(L);
    CX_WARN("%s",cxStringBody(log));
    return 0;
}

static cxInt cxLuaRetain(lua_State *L)
{
    if(lua_islightuserdata(L, 1)){
        CX_RETAIN(lua_touserdata(L, 1));
    }
    return 0;
}

static cxInt cxLuaRelease(lua_State *L)
{
    if(lua_islightuserdata(L, 1)){
        CX_RELEASE(lua_touserdata(L, 1));
    }
    return 0;
}

static cxInt cxLuaAutoFree(lua_State *L)
{
    cxAny ptr = NULL;
    if(lua_islightuserdata(L, 1)){
        ptr = CX_AUTOFREE(lua_touserdata(L, 1));
    }
    if(ptr == NULL){
        lua_pushlightuserdata(L, ptr);
    }else{
        lua_pushnil(L);
    }
    return 1;
}

const luaL_reg global_functions [] = {
    {"cxLogger", cxLuaLogger},
    {"cxError", cxLuaError},
    {"cxWarn", cxLuaWarn},
    {"cxRetain", cxLuaRetain},
    {"cxRelease", cxLuaRelease},
    {"cxAutoFree", cxLuaAutoFree},
    {NULL, NULL}
};

void cxLuaLoaderFuncs(cxAny loader)
{
    cxLuaLoader this = loader;
    luaL_register(this->L, "_G", global_functions);
}







