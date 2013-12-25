//
//  cxConfig.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxConfig.h"
#include "cxXMLScript.h"

lua_State *gL = NULL;

static cxInt cxObjectLuaGetTag(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    lua_pushinteger(L, cxObjectGetTag(this));
    return 1;
}

static cxInt cxObjectLuaSetTag(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    cxInt tag = luaL_checkinteger(L, 2);
    cxObjectSetTag(this, tag);
    return 0;
}

static cxInt cxObjectLuaGC(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    CX_RELEASE(this);
    return 0;
}

cxInt cxObjectLuaAppendEvent(lua_State *L)
{
    return 0;
}

static cxInt cxObjectLuaToString(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    lua_pushstring(L, this->cxType);
    return 1;
}

const luaL_Reg cxObjectInstanceMethods[] = {
    {"__tostring",cxObjectLuaToString},
    CX_LUA_PROPERTY(cxObject, Tag)
    CX_LUA_ON_EVENT(cxObject)
    CX_LUA_GC_FUNC(cxObject)
    {NULL,NULL},
    {NULL,NULL},
};

const luaL_Reg cxObjectTypeMethods[] = {
    CX_LUA_TYPE(cxObject)
};

static void cxLuaRegisterMethods(const luaL_Reg *methods)
{
    const luaL_Reg *curr = NULL;
    cxInt i = 0;
    while (true) {
        curr = &methods[i++];
        CX_BREAK(curr->func == NULL && curr->name == NULL);
    }
    curr = &methods[i];
    if(curr->func != NULL && strcmp(curr->name, "super") == 0){
        cxLuaRegisterMethods((const luaL_Reg *)curr->func);
    }
    luaL_register(gL, NULL, methods);
}

void cxLuaRegisterTypeFunc(cxConstType type,cxConstChars name, lua_CFunction func)
{
    luaL_getmetatable(gL, type);
    CX_ASSERT(lua_istable(gL, -1), "type %s not metatable");
    lua_getfield(gL, -1, "function");
    CX_ASSERT(lua_istable(gL, -1), "type %s function not find");
    lua_pushcfunction(gL, func);
    lua_setfield(gL, -2, name);
    lua_pop(gL, 2);
}

lua_State *cxLuaLoad(cxConstType name, const luaL_Reg *cMethods,const luaL_Reg *tMethods)
{
    CX_ASSERT(cMethods != NULL && tMethods != NULL, "args error");
    luaL_newmetatable(gL, name);
    cxLuaRegisterMethods(cMethods);

    lua_pushvalue(gL,-1);
    lua_setfield(gL, -2, "__index");
    
    lua_newtable(gL);
    lua_setfield(gL, -2, "function");
    
    luaL_register(gL, name, tMethods);
    lua_pop(gL, 2);
    return gL;
}

void cxObjectTypeInit()
{
    CX_LUA_LOAD_TYPE(cxObject);
}

void cxObjectReadAttr(cxReaderAttrInfo *info)
{
    cxObject this = info->object;
    cxObjectSetTag(this,cxXMLReadIntAttr(info, "cxObject.tag", this->cxTag));
}

void cxObjectAutoInit(cxObject this)
{
    this->cxBase = cxBaseTypeObject;
    CX_METHOD_OVERRIDE(this->ReadAttr, cxObjectReadAttr);
}

void cxObjectAutoFree(cxObject this)
{
    CX_METHOD_RELEASE(this->ReadAttr);
}

void cxObjectSetReadAttrFunc(cxAny obj,cxReadAttrFunc func)
{
    cxObject this = obj;
    CX_METHOD_OVERRIDE(this->ReadAttr, func);
}

void cxObjectReadAttrRun(cxReaderAttrInfo *info)
{
    cxObject this = info->object;
    CX_METHOD_FIRE(NULL, this->ReadAttr,info);
}

void cxObjectSetRoot(cxAny obj,cxAny root)
{
    cxObject this = obj;
    this->cxRoot = root;
}

cxBool cxObjectIsBaseType(cxAny pobj,cxBaseType type)
{
    cxObject this = pobj;
    CX_RETURN(this == NULL, false);
    return this->cxBase == type;
}

cxConstType cxObjectType(cxAny pobj)
{
    cxObject this = pobj;
    CX_RETURN(this == NULL, false);
    return this->cxType;
}

cxBool cxObjectIsType(cxAny pobj,cxConstType type)
{
    cxObject this = pobj;
    CX_RETURN(this == NULL, false);
    return this->cxType == type;
}

cxAny cxObjectRoot(cxAny obj)
{
    CX_RETURN(obj == NULL, NULL);
    cxObject this = obj;
    return this->cxRoot;
}

void cxObjectSetTag(cxAny obj,cxInt tag)
{
    cxObject this = obj;
    this->cxTag = tag;
}

cxInt cxObjectGetTag(cxAny obj)
{
    cxObject this = obj;
    return this->cxTag;
}
