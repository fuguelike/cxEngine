//
//  cxLuaLoader.c
//  cxEngine
//
//  Created by xuhua on 12/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxUtil.h>
#include "cxLuaLoader.h"
#include "cxLuaFuncs.h"
#include "cxLuaView.h"

static void *cxLuaLoaderAlloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
    if(nsize == 0){
        allocator->free(ptr);
        return NULL;
    }else{
        return allocator->realloc(ptr,nsize);
    }
}

static void cxLuaLoaderAddSearchPath(cxLuaLoader this,const char* path)
{
    CX_ASSERT(path != NULL, "path error");
    lua_getglobal(this->L, "package");
    lua_getfield(this->L, -1, "path");
    const char* cur_path =  lua_tostring(this->L, -1);
    lua_pushfstring(this->L, "%s;%s/?.lua", cur_path, path);
    lua_setfield(this->L, -3, "path");
    lua_pop(this->L, 2);
}

static int cxLuaLoaderLuaLoader(lua_State *L)
{
    
    cxConstChars file = luaL_checkstring(L, 1);
    cxString data = cxEngineGetLuaScript(file);
    if(data == NULL){
        luaL_error(L, "error loading file %s error",file);
    }else{
        luaL_loadbuffer(L, cxStringBody(data), cxStringLength(data), file);
    }
    return 1;
}

void cxLuaLoaderAddLuaLoader(cxLuaLoader this,lua_CFunction func)
{
    CX_ASSERT(func != NULL, "func args error");
    lua_getglobal(this->L, "package");
    lua_getfield(this->L, -1, "loaders");
    lua_pushcfunction(this->L, func);
    for (int i = lua_objlen(this->L, -2) + 1; i > 2; --i) {
        lua_rawgeti(this->L, -2, i - 1);
        lua_rawseti(this->L, -3, i);
    }
    lua_rawseti(this->L, -2, 2);
    lua_setfield(this->L, -2, "loaders");
    lua_pop(this->L, 1);
}

static int cxLuaLoaderPanic(lua_State*L)
{
    CX_ERROR("PANIC: unprotected error in call to Lua API (%s)\n",lua_tostring(L, -1));
    return 0;
}

static void cxLuaLoaderTypes(cxLuaLoader this)
{
    cxLuaViewLoad(this);
}

cxBool cxLuaLoaderCheckFunc(cxLuaLoader this,cxConstChars name)
{
    lua_getglobal(this->L, name);
    cxBool rv = lua_isfunction(this->L, -1);
    if(!rv){
        CX_ERROR("lua get global function (%s) error",name);
    }
    return rv;
}

cxBool cxLuaLoaderCallFunc(cxLuaLoader this,cxInt an,cxInt rn)
{
    if(lua_pcall(this->L, an, rn, 0) != 0){
        CX_ERROR("lua call global function error:%s",lua_tostring(this->L, -1));
        return false;
    }
    return true;
}

cxBool cxLuaLoaderRun(cxLuaLoader this,cxString code)
{
    CX_ASSERT(code != NULL && cxStringLength(code) > 0, "code args error");
    if(luaL_dostring(this->L, cxStringBody(code)) != 0){
        CX_ERROR("cxLuaLoader run code error:%s",lua_tostring(this->L, -1));
        return false;
    }
    return true;
}

CX_OBJECT_INIT(cxLuaLoader, cxObject)
{
    this->L = lua_newstate(cxLuaLoaderAlloc, this);
    CX_ASSERT(this->L != NULL, "new lua state error");
    lua_atpanic(this->L, cxLuaLoaderPanic);
    luaL_openlibs(this->L);
    cxString path = cxDocumentPath(NULL);
    cxLuaLoaderAddSearchPath(this, cxStringBody(path));
    cxLuaLoaderAddLuaLoader(this, cxLuaLoaderLuaLoader);
    cxLuaLoaderFuncs(this);
    cxLuaLoaderTypes(this);
}
CX_OBJECT_FREE(cxLuaLoader, cxObject)
{
    lua_close(this->L);
}
CX_OBJECT_TERM(cxLuaLoader, cxObject)
















