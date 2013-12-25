//
//  cxList.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxList.h"

static cxInt cxListLuaAppend(lua_State *L)
{
    CX_LUA_DEF_THIS(cxList);
    cxAny any = cxLuaObjectValue(L, 2);
    cxListElement *ret = cxListAppend(this, any);
    lua_pushlightuserdata(L, ret);
    return 1;
}

static cxInt cxListLuaPrepend(lua_State *L)
{
    CX_LUA_DEF_THIS(cxList);
    cxAny any = cxLuaObjectValue(L, 2);
    cxListElement *ret = cxListPrepend(this, any);
    lua_pushlightuserdata(L, ret);
    return 1;
}

static cxInt cxListLuaFirst(lua_State *L)
{
    CX_LUA_DEF_THIS(cxList);
    cxAny any = cxListFirst(this);
    CX_LUA_PUSH_OBJECT(any);
    return 1;
}

static cxInt cxListLuaLast(lua_State *L)
{
    CX_LUA_DEF_THIS(cxList);
    cxAny any = cxListLast(this);
    CX_LUA_PUSH_OBJECT(any);
    return 1;
}

static cxInt cxListLuaLength(lua_State *L)
{
    CX_LUA_DEF_THIS(cxList);
    lua_pushnumber(L, cxListLength(this));
    return 1;
}

static cxInt cxListLuaRemove(lua_State *L)
{
    CX_LUA_DEF_THIS(cxList);
    cxListElement *ele = lua_touserdata(L, 2);
    cxListRemove(this, ele);
    return 0;
}

static cxInt cxListLuaClean(lua_State *L)
{
    CX_LUA_DEF_THIS(cxList);
    cxListClean(this);
    return 0;
}

const luaL_Reg cxListInstanceMethods[] = {
    {"append",  cxListLuaAppend},
    {"prepend", cxListLuaPrepend},
    {"first",   cxListLuaFirst},
    {"last",    cxListLuaLast},
    {"length",  cxListLuaLength},
    {"remove",  cxListLuaRemove},
    {"clean",   cxListLuaClean},
    CX_LUA_SUPER(cxObject)
};

const luaL_Reg cxListTypeMethods[] = {
    CX_LUA_TYPE(cxList)
};

void cxListTypeInit()
{
    CX_LUA_LOAD_TYPE(cxList);
}

CX_OBJECT_INIT(cxList, cxObject)
{
    
}
CX_OBJECT_FREE(cxList, cxObject)
{
    cxListClean(this);
}
CX_OBJECT_TERM(cxList, cxObject)

cxListElement *cxListAppend(cxList list,cxAny any)
{
    cxListElement *element = allocator->malloc(sizeof(cxListElement));
    element->any = any;
    DL_APPEND(list->listptr, element);
    CX_RETAIN(any);
    return element;
}

cxListElement *cxListPrepend(cxList list,cxAny any)
{
    cxListElement *element = allocator->malloc(sizeof(cxListElement));
    element->any = any;
    DL_PREPEND(list->listptr, element);
    CX_RETAIN(any);
    return element;
}

cxInt cxListLength(cxList list)
{
    cxInt count = 0;
    cxListElement *ele = NULL;
    DL_COUNT(list->listptr, ele, count);
    return count;
}

cxListElement *cxListFirst(cxList list)
{
    return list->listptr;
}

cxListElement *cxListLast(cxList list)
{
    return list->listptr == NULL ? NULL : list->listptr->prev;
}

void cxListRemove(cxList list,cxListElement *element)
{
    DL_DELETE(list->listptr, element);
    CX_RELEASE(element->any);
    allocator->free(element);
}

void cxListClean(cxList list)
{
    cxListElement *ele = NULL,*tmp=NULL;
    DL_FOREACH_SAFE(list->listptr,ele,tmp){
        cxListRemove(list, ele);
    }
}





