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

static cxInt cxObjectGetRefCount(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    lua_pushinteger(L, this->cxRefcount);
    return 1;
}

const luaL_Reg cxObjectInstanceMethods[] = {
    {"getTag",cxObjectLuaGetTag},
    {"setTag",cxObjectLuaSetTag},
    {"getRefCount",cxObjectGetRefCount},
    {"__gc",cxObjectLuaGC},
    {NULL,NULL},
    {NULL,NULL},
};

void cxLuaRegisterMethods(lua_State *L,const luaL_Reg *methods)
{
    const luaL_Reg *curr = NULL;
    cxInt i = 0;
    while (true) {
        curr = &methods[i++];
        CX_BREAK(curr->func == NULL && curr->name == NULL);
    }
    curr = &methods[i];
    if(curr->func != NULL && strcmp(curr->name, "super") == 0){
        cxLuaRegisterMethods(L, (const luaL_Reg *)curr->func);
    }
    luaL_register(L, NULL, methods);
}

cxInt cxLuaLoad(cxConstType name, const luaL_Reg *cMethods,const luaL_Reg *tMethods)
{
    CX_ASSERT(cMethods != NULL && tMethods != NULL, "args error");
    cxEngine engine = cxEngineInstance();
    
    luaL_newmetatable(engine->L, name);

    cxLuaRegisterMethods(engine->L, cMethods);

    lua_pushvalue(engine->L,-1);
    lua_setfield(engine->L, -2, "__index");
    
    luaL_register(engine->L, name, tMethods);
    return 1;
}

void cxObjectTypeInit()
{
    CX_LUA_LOAD_TYPE(cxObject);
}

void cxObjectReadAttr(cxAny pobj,cxAny newobj, xmlTextReaderPtr reader)
{
    cxObject this = newobj;
    cxObjectSetTag(this,cxXMLReadIntAttr(reader, NULL, "cxObject.tag", this->cxTag));
}

void cxObjectAutoInit(cxObject this)
{
    this->cxBase = cxBaseTypeObject;
    CX_METHOD_SET(this->ReadAttr, cxObjectReadAttr);
}

void cxObjectAutoFree(cxObject this)
{
    
}

void cxObjectSetReadAttrFunc(cxAny obj,cxReadAttrFunc func)
{
    cxObject this = obj;
    CX_METHOD_SET(this->ReadAttr, func);
}

void cxObjectReadAttrRun(cxAny obj,cxAny pobj,xmlTextReaderPtr reader)
{
    cxObject this = obj;
    CX_METHOD_RUN(this->ReadAttr, pobj, obj,reader);
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
