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
    const char* curpath = lua_tostring(this->L, -1);
    lua_pop(this->L, 1);
    lua_pushfstring(this->L, "%s;%s/?.lua", curpath, path);
    lua_setfield(this->L, -2, "path");
    lua_pop(this->L, 1);
}

static int cxLuaLoaderPanic(lua_State*L)
{
    CX_ERROR("PANIC: unprotected error in call to Lua API (%s)\n",lua_tostring(L,-1));
    return 0;
}

static void cxLuaLoaderTypes(cxLuaLoader this)
{
    
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
    if(!cxLuaLoaderCheckFunc(this, "load")){
        return false;
    }
    lua_pushstring(this->L, cxStringBody(code));
    if(!cxLuaLoaderCallFunc(this, 1, 1)){
        return false;
    }
    if(lua_isfunction(this->L, -1)){
        return cxLuaLoaderCallFunc(this,0,0);
    }
    CX_ERROR("run code %s error",cxStringBody(code));
    return false;
}

CX_OBJECT_INIT(cxLuaLoader, cxObject)
{
    this->L = lua_newstate(cxLuaLoaderAlloc, this);
    CX_ASSERT(this->L != NULL, "new lua state error");
    lua_atpanic(this->L, cxLuaLoaderPanic);
    luaL_openlibs(this->L);
    cxString path = cxDocumentPath(NULL);
    cxLuaLoaderAddSearchPath(this, cxStringBody(path));
    cxLuaLoaderTypes(this);
}
CX_OBJECT_FREE(cxLuaLoader, cxObject)
{
    lua_close(this->L);
}
CX_OBJECT_TERM(cxLuaLoader, cxObject)
















