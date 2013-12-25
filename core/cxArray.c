//
//  cxArray.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxArray.h"

static cxInt cxArrayLuaLength(lua_State *L)
{
    CX_LUA_DEF_THIS(cxArray);
    lua_pushnumber(L, cxArrayLength(this));
    return 1;
}

static cxInt cxArrayLuaAtIndex(lua_State *L)
{
    CX_LUA_DEF_THIS(cxArray);
    cxInt idx = lua_tointeger(L, 2);
    cxAny any = cxArrayAtIndex(this, idx);
    CX_LUA_PUSH_OBJECT(any);
    return 1;
}

static cxInt cxArrayLuaAppend(lua_State *L)
{
    CX_LUA_DEF_THIS(cxArray);
    cxAny any = cxLuaGetObject(L, 2);
    cxArrayAppend(this, any);
    return 0;
}

static cxInt cxArrayLuaEach(lua_State *L)
{
    CX_LUA_DEF_THIS(cxArray);
    CX_ARRAY_FOREACH(this, e){
        cxAny any = cxArrayObject(e);
        lua_pushvalue(L, 2);
        CX_LUA_PUSH_OBJECT(any);
        lua_call(L, 1, 0);
    }
    return 0;
}

static cxInt cxArrayLuaRemove(lua_State *L)
{
    CX_LUA_DEF_THIS(cxArray);
    if(lua_isuserdata(L, 2)){
        CX_LUA_GET_ANY(cxObject, any, 2);
        cxArrayRemove(this, any);
        return 0;
    }
    if(lua_isnumber(L, 2)){
        cxArrayRemoveAtIndex(this, lua_tointeger(L, 2));
        return 0;
    }
    luaL_error(L, "args is object or int number");
    return 0;
}

static cxInt cxArrayLuaClean(lua_State *L)
{
    CX_LUA_DEF_THIS(cxArray);
    cxArrayClean(this);
    return 0;
}

const luaL_Reg cxArrayInstanceMethods[] = {
    {"length",cxArrayLuaLength},
    {"at",cxArrayLuaAtIndex},
    {"append",cxArrayLuaAppend},
    {"remove",cxArrayLuaRemove},
    {"each",cxArrayLuaEach},
    {"clean",cxArrayLuaClean},
    CX_LUA_SUPER(cxObject)
};

const luaL_Reg cxArrayTypeMethods[] = {
    CX_LUA_TYPE(cxArray)
};

void cxArrayTypeInit()
{
    CX_LUA_LOAD_TYPE(cxArray);
}

CX_OBJECT_INIT(cxArray, cxObject)
{
    utarray_new(this->utArray, &ut_ptr_icd);
}
CX_OBJECT_FREE(cxArray, cxObject)
{
    cxArrayClean(this);
    utarray_free(this->utArray);
}
CX_OBJECT_TERM(cxArray, cxObject)

void cxArrayClean(cxArray array)
{
    CX_ARRAY_FOREACH(array, e) {
        cxAny any = cxArrayObject(e);
        CX_RELEASE(any);
    }
    utarray_clear(array->utArray);
}

void cxArrayUpdate(cxArray array,cxAny nAny,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxPointer *ele = (cxPointer *)utarray_eltptr(array->utArray, index);
    if(ele == NULL){
        return;
    }
    cxAny oAny = cxArrayObject(ele);
    if(oAny == nAny){
        return;
    }
    CX_RELEASE(oAny);
    *ele = nAny;
    CX_RETAIN(nAny);
}

void cxArrayAppends(cxArray array, cxArray data)
{
    CX_ARRAY_FOREACH(data, ele) {
        cxAny obj = cxArrayObject(ele);
        cxArrayAppend(array, obj);
    }
}

void cxArrayAppend(cxArray array, cxAny any)
{
    utarray_push_back(array->utArray, &any);
    CX_RETAIN(any);
}

cxIndex cxArrayObjectIndex(cxArray array,cxAny any)
{
    CX_ARRAY_FOREACH(array, e) {
        cxAny tmp = cxArrayObject(e);
        if(tmp == any) {
            return (cxIndex)utarray_eltidx(array->utArray,e);
        }
    }
    return CX_INVALID_INDEX;
}

void cxArrayRemove(cxArray array,cxAny any)
{
    int index = cxArrayObjectIndex(array, any);
    if(index != CX_INVALID_INDEX) {
        utarray_erase(array->utArray, index, 1);
        CX_RELEASE(any);
    }
}

void cxArrayRemoveAtIndex(cxArray array,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxAny any = cxArrayAtIndex(array, index);
    utarray_erase(array->utArray, index, 1);
    CX_RELEASE(any);
}

cxAny cxArrayAtIndex(cxArray array,cxIndex index)
{
    CX_ASSERT(index >= 0 && index < cxArrayLength(array), "index invalid");
    cxPointer *e = (cxPointer *)utarray_eltptr(array->utArray, index);
    return cxArrayObject(e);
}

cxAny cxArrayFirst(cxArray array)
{
    cxPointer *e = (cxPointer *)utarray_front(array->utArray);
    return e != NULL ? cxArrayObject(e): NULL;
}

cxAny cxArrayLast(cxArray array)
{
    cxPointer *e = (cxPointer *)utarray_back(array->utArray);
    return e != NULL ? cxArrayObject(e): NULL;
}

void cxArrayRemoveFirst(cxArray array)
{
    cxAny any = cxArrayFirst(array);
    if(any != NULL) {
        utarray_erase(array->utArray, 0, 1);
        CX_RELEASE(any);
    }
}

void cxArrayRemoveLast(cxArray array)
{
    cxAny any = cxArrayLast(array);
    if(any != NULL) {
        utarray_pop_back(array->utArray);
        CX_RELEASE(any);
    }
}



