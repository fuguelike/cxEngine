//
//  cxTypes.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxHash.h"
#include "cxTypes.h"
#include "cxArray.h"
#include "cxString.h"
#include "cxNumber.h"

void cxLuaPushVec2fv(lua_State *L,cxVec2f pos)
{
    lua_newtable(L);
    lua_pushnumber(L, pos.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, pos.y);
    lua_setfield(L, -2, "y");
}

void cxLuaPushSize2fv(lua_State *L,cxSize2f size)
{
    lua_newtable(L);
    lua_pushnumber(L, size.w);
    lua_setfield(L, -2, "w");
    lua_pushnumber(L, size.h);
    lua_setfield(L, -2, "h");
}

void cxLuaPushColor4f(lua_State *L,cxColor4f color)
{
    lua_newtable(L);
    lua_pushnumber(L, color.r);
    lua_setfield(L, -2, "r");
    lua_pushnumber(L, color.g);
    lua_setfield(L, -2, "g");
    lua_pushnumber(L, color.b);
    lua_setfield(L, -2, "b");
    lua_pushnumber(L, color.a);
    lua_setfield(L, -2, "a");
}

cxBool cxLuaBoolValue(lua_State *L,cxInt n,cxBool d)
{
    cxBool rv = d;
    cxInt top = lua_gettop(L);
    if(top >= n && lua_isboolean(L, n)){
        rv = lua_toboolean(L, n);
    }
    return rv;
}

cxInt cxLuaIntValue(lua_State *L,cxInt n,cxInt d)
{
    cxInt rv = d;
    cxInt top = lua_gettop(L);
    if(top >= n && lua_isnumber(L, n)){
        rv = lua_tointeger(L, n);
    }
    return rv;
}

cxConstChars cxLuaStringValue(lua_State *L,cxInt n,cxConstChars d)
{
    cxConstChars rv = d;
    cxInt top = lua_gettop(L);
    if(top >= n && lua_isstring(L, n)){
        rv = lua_tostring(L, n);
    }
    return rv;
}

cxFloat cxLuaFloatValue(lua_State *L,cxInt n,cxFloat d)
{
    cxFloat rv = d;
    cxInt top = lua_gettop(L);
    if(top >= n && lua_isnumber(L, n)){
        rv = lua_tonumber(L, n);
    }
    return rv;
}

cxVec2f cxLuaVec2fValue(lua_State *L,cxInt n,cxVec2f d)
{
    cxVec2f v = d;
    if(!lua_istable(L, n)){
        return v;
    }
    lua_getfield(L, n, "x");
    v.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, n, "y");
    v.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return v;
}

cxColor4f cxLuaColor4fValue(lua_State *L,cxInt n,cxColor4f d)
{
    cxColor4f v = d;
    if(!lua_istable(L, n)){
        return v;
    }
    lua_getfield(L, n, "r");
    if(lua_isnumber(L, -1)){
        v.r = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);
    lua_getfield(L, n, "g");
    if(lua_isnumber(L, -1)){
        v.g = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);
    lua_getfield(L, n, "b");
    if(lua_isnumber(L, -1)){
        v.b = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);
    lua_getfield(L, n, "a");
    if(lua_isnumber(L, -1)){
        v.a = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);
    return v;
}

cxSize2f cxLuaSize2fValue(lua_State *L,cxInt n,cxSize2f d)
{
    cxSize2f v = d;
    if(!lua_istable(L, n)){
        return v;
    }
    lua_getfield(L, n, "w");
    v.w = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, n, "h");
    v.h = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return v;
}

cxAny cxLuaObjectValue(lua_State *L,cxInt n)
{
    if(lua_isnumber(L, n)){
        lua_Number num = lua_tonumber(L, n);
        return CX_LUA_NUMBER_IS_INT(num) ? cxNumberInt(num) : cxNumberFloat(num);
    }
    if(lua_isstring(L, n)){
        return cxStringConstChars(lua_tostring(L, n));
    }
    if(lua_isuserdata(L, n)){
        return CX_LUA_GET_PTR(2);
    }
    return NULL;
}

CX_OBJECT_INIT(cxTypes, cxObject)
{
    
}
CX_OBJECT_FREE(cxTypes, cxObject)
{
    CX_RELEASE(this->any);
}
CX_OBJECT_TERM(cxType, cxObject)

cxTypes cxStringTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesString;
    return this;
}

cxTypes cxHashTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesHash;
    this->any = CX_ALLOC(cxHash);
    return this;
}

cxTypes cxNumberTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesNumber;
    return this;
}

cxTypes cxArrayTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesArray;
    this->any = CX_ALLOC(cxArray);
    return this;
}

cxTypes cxDBTypesCreate(cxAny db)
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesDB;
    CX_RETAIN_SWAP(this->any, db);
    return this;
}

