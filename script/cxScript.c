//
//  cxScript.c
//  cxEngine
//
//  Created by xuhua on 11/30/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxUtil.h>
#include "cxScript.h"

static void *cxScriptAlloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
    if(nsize == 0){
        allocator->free(ptr);
        return NULL;
    }else{
        return allocator->realloc(ptr,nsize);
    }
}

static void cxScriptAddSearchPath(lua_State *L,const char* path)
{
    CX_ASSERT(path != NULL, "path error");
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path");
    const char* curpath = lua_tostring(L, -1);
    lua_pop(L, 1);
    lua_pushfstring(L, "%s;%s/?.lua", curpath, path);
    lua_setfield(L, -2, "path");
    lua_pop(L, 1);
}

static int cxScriptPanic(lua_State*L)
{
    CX_ERROR("PANIC: unprotected error in call to Lua API (%s)\n",lua_tostring(L,-1));
    return 0;
}

CX_OBJECT_INIT(cxScript, cxObject)
{
    this->L = lua_newstate(cxScriptAlloc, this);
    CX_ASSERT(this->L != NULL, "new lua state error");
    lua_atpanic(this->L, cxScriptPanic);
    luaL_openlibs(this->L);
    cxString path = cxDocumentPath(NULL);
    cxScriptAddSearchPath(this->L, cxStringBody(path));
}
CX_OBJECT_FREE(cxScript, cxObject)
{
    lua_close(this->L);
}
CX_OBJECT_TERM(cxScript, cxObject)

cxBool cxScriptAppend(cxAny ps,cxString code)
{
    return false;
}

cxBool cxScriptLoad(cxAny ps,cxString code)
{
    cxScript this = ps;
    cxInt ret = luaL_loadstring(this->L, cxStringBody(code));
    if(ret != 0){
        CX_ERROR("load string error:%s",lua_tostring(this->L, -1));
        return false;
    }
    if(lua_pcall(this->L, 0, 0, 0) != 0){
        CX_ERROR("lua pcall error:%s",lua_tostring(this->L, -1));
        return false;
    }
    return cxScriptInit(ps);
}

cxBool cxScriptCheckFunc(cxAny ps,cxConstChars name)
{
    cxScript this = ps;
    lua_getglobal(this->L, name);
    cxBool rv = lua_isfunction(this->L, -1);
    if(!rv){
        CX_ERROR("lua get global function (%s) error",name);
    }
    return rv;
}

cxBool cxScriptCallFunc(cxAny ps,cxInt an,cxInt rn)
{
    cxScript this = ps;
    if(lua_pcall(this->L, an, rn, 0) != 0){
        CX_ERROR("lua call global function error:%s",lua_tostring(this->L, -1));
        return false;
    }
    return true;
}

cxBool cxScriptInit(cxAny ps)
{
    return cxScriptCheckFunc(ps, "onInit") && cxScriptCallFunc(ps, 0, 0);
}

cxBool cxScriptMain(cxAny ps)
{
    return cxScriptCheckFunc(ps, "onMain") && cxScriptCallFunc(ps, 0, 0);
}

cxBool cxScriptFree(cxAny ps)
{
    return cxScriptCheckFunc(ps, "onFree") && cxScriptCallFunc(ps, 0, 0);
}
















