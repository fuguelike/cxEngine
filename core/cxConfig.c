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

CX_LUA_METHOD_BEGIN(cxObject)
    {"retain",cxObjectLuaRetain},
    {"release",cxObjectLuaRelease},
    {"autoRelease",cxObjectLuaAutoRelease},
    CX_LUA_PROPERTY(cxObject, Tag),
CX_LUA_METHOD_END(cxObject)

lua_State *cxLuaLoad(cxConstType name, const luaL_Reg *methods)
{
    luaL_openlib(gL, name, methods, 0);
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
