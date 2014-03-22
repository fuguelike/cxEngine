//
//  cxObject.c
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxXMLScript.h"
#include "cxObject.h"

lua_State *gL = NULL;

static cxInt cxObjectLuaGetTag(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    lua_pushinteger(L, cxObjectTag(this));
    return 1;
}

static cxInt cxObjectLuaSetTag(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    cxInt tag = luaL_checkinteger(L, 2);
    cxObjectSetTag(this, tag);
    return 0;
}

static cxInt cxObjectLuaRetain(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    CX_RETAIN(this);
    return 0;
}

static cxInt cxObjectLuaRelease(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    CX_RELEASE(this);
    return 0;
}

static cxInt cxObjectLuaAutoRelease(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    CX_AUTOFREE(this);
    return 1;
}

static cxInt cxObjectLuaGetType(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    lua_pushstring(L, this->cxType);
    return 1;
}

static cxInt cxObjectLuaGetBind(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    if(this->cxBind > 0){
        lua_getref(L, this->cxBind);
    }else{
        lua_pushnil(L);
    }
    return 1;
}

CX_LUA_METHOD_BEG(cxObject)
    {"GetBind",cxObjectLuaGetBind},
    {"GetType",cxObjectLuaGetType},
    {"Retain",cxObjectLuaRetain},
    {"Release",cxObjectLuaRelease},
    {"AutoRelease",cxObjectLuaAutoRelease},
    CX_LUA_PROPERTY(cxObject, Tag),
CX_LUA_METHOD_END(cxObject)

void cxLuaLoad(cxConstType name, const luaL_Reg *methods)
{
    luaL_openlib(gL, name, methods, 0);
    lua_pop(gL, 1);
}

void __cxObjectTypeInit()
{
    CX_LUA_LOAD_TYPE(cxObject);
}

void __cxObjectAutoInit(cxObject this)
{
    this->cxBase = cxBaseTypeObject;
    CX_METHOD_OVERRIDE(this->ReadAttr, cxObjectReadAttr);
}

void __cxObjectAutoFree(cxObject this)
{
    if(this->cxBind > 0){
        lua_unref(gL, this->cxBind);
        this->cxBind = 0;
    }
    CX_METHOD_RELEASE(this->ReadAttr);
}

cxInt cxObjectBind(cxAny obj)
{
    CX_RETURN(obj == NULL, 0);
    cxObject this = obj;
    return this->cxBind;
}

void cxObjectReadAttr(cxReaderAttrInfo *info)
{
    cxObject this = info->object;
    cxObjectSetTag(this,cxXMLReadIntAttr(info, "cxObject.tag", this->cxTag));
    cxXMLBindObject(info);
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
    CX_RETURN(this == NULL, NULL);
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

cxInt cxObjectTag(cxAny obj)
{
    cxObject this = obj;
    return this->cxTag;
}


//auto release memory block
CX_OBJECT_INIT(cxMemory, cxObject)
{
    
}
CX_OBJECT_FREE(cxMemory, cxObject)
{
    allocator->free(this->pointer);
}
CX_OBJECT_TERM(cxMemory, cxObject)

cxAny cxMemoryCreate(cxInt size)
{
    cxMemory this = CX_CREATE(cxMemory);
    this->pointer = allocator->malloc(size);
    memset(this->pointer, 0, size);
    return this->pointer;
}



