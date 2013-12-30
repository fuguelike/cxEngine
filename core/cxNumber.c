//
//  cxNumber.c
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTypes.h"
#include "cxNumber.h"

static cxInt cxNumberLuaNewInt(lua_State *L)
{
    cxInt value =  luaL_checkinteger(L, 1);
    cxNumber this = cxNumberInt(value);
    lua_pushlightuserdata(L, this);
    CX_LUA_RET_THIS(cxNumber);
}

static cxInt cxNumberLuaToInt(lua_State *L)
{
    CX_LUA_DEF_THIS(cxNumber);
    if(this->type != cxNumberTypeInt){
        luaL_error(L, "number type error");
        lua_pushnil(L);
    }else{
        lua_pushinteger(L, this->value.vi);
    }
    return 1;
}

static cxInt cxNumberLuaNewFloat(lua_State *L)
{
    cxFloat value =  luaL_checknumber(L, 1);
    cxNumber this = cxNumberFloat(value);
    lua_pushlightuserdata(L, this);
    CX_LUA_RET_THIS(cxNumber);
}

static cxInt cxNumberLuaToFloat(lua_State *L)
{
    CX_LUA_DEF_THIS(cxNumber);
    if(this->type != cxNumberTypeFloat){
        luaL_error(L, "number type error");
        lua_pushnil(L);
    }else{
        lua_pushnumber(L, this->value.vf);
    }
    return 1;
}

static cxInt cxNumberLuaNewColor4f(lua_State *L)
{
    cxInt top = lua_gettop(L);
    cxFloat r = (top >=1 && lua_isnumber(L, 1)) ? lua_tonumber(L, 1) : 1.0f;
    cxFloat g = (top >=2 && lua_isnumber(L, 2)) ? lua_tonumber(L, 2) : 1.0f;
    cxFloat b = (top >=3 && lua_isnumber(L, 3)) ? lua_tonumber(L, 3) : 1.0f;
    cxFloat a = (top >=4 && lua_isnumber(L, 4)) ? lua_tonumber(L, 4) : 1.0f;
    cxNumber this = cxNumberColor4f(cxColor4bv(r, g, b, a));
    lua_pushlightuserdata(L, this);
    CX_LUA_RET_THIS(cxNumber);
}

static cxInt cxNumberLuaToColor4f(lua_State *L)
{
    CX_LUA_DEF_THIS(cxNumber);
    if(this->type != cxNumberTypeColor4f){
        luaL_error(L, "number type error");
        lua_pushnil(L);
    }else{
        cxLuaPushColor4f(L, this->value.color4f);
    }
    return 1;
}

static cxInt cxNumberLuaNewVec2f(lua_State *L)
{
    cxFloat x = luaL_checknumber(L, 1);
    cxFloat y = luaL_checknumber(L, 2);
    cxNumber this = cxNumberVec2f(cxVec2fv(x, y));
    lua_pushlightuserdata(L, this);
    CX_LUA_RET_THIS(cxNumber);
}

static cxInt cxNumberLuaNewBool(lua_State *L)
{
    cxBool b = lua_toboolean(L, 1);
    cxNumber this = cxNumberBool(b);
    lua_pushlightuserdata(L, this);
    CX_LUA_RET_THIS(cxNumber);
}

static cxInt cxNumberLuaNewSize2f(lua_State *L)
{
    cxFloat w = luaL_checknumber(L, 1);
    cxFloat h = luaL_checknumber(L, 2);
    cxNumber this = cxNumberSize2f(cxSize2fv(w, h));
    lua_pushlightuserdata(L, this);
    CX_LUA_RET_THIS(cxNumber);
}

CX_LUA_METHOD_BEG(cxNumber)
    {"newInt",cxNumberLuaNewInt},
    {"toInt",cxNumberLuaToInt},
    {"newFloat",cxNumberLuaNewFloat},
    {"toFloat",cxNumberLuaToFloat},
    {"newColor4f",cxNumberLuaNewColor4f},
    {"toColor4f",cxNumberLuaToColor4f},
    {"newVec2f",cxNumberLuaNewVec2f},
    {"newBool",cxNumberLuaNewBool},
    {"newSize2f",cxNumberLuaNewSize2f},
CX_LUA_METHOD_END(cxNumber)

void cxNumberTypeInit()
{
    CX_LUA_LOAD_TYPE(cxNumber);
}

CX_OBJECT_INIT(cxNumber, cxObject)
{
    
}
CX_OBJECT_FREE(cxNumber, cxObject)
{
    
}
CX_OBJECT_TERM(cxNumber, cxObject)

cxBool cxNumberToBool(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeBool, "type error");
    return this->value.bv;
}

cxBox4i cxNumberToBox4i(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeBox4i, "type error");
    return this->value.box4i;
}

cxVec3f cxNumberToVec3f(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeVec3f, "type error");
    return this->value.vec3f;
}

cxColor4fRange cxNumberToColor4fRange(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeColor4fRange, "type error");
    return this->value.vcr;
}

cxVec2fRange cxNumberToVec2fRange(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeVec2fRange, "type error");
    return this->value.vvr;
}

cxFloatRange cxNumberToFloatRange(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeFloatRange, "type error");
    return this->value.vfr;
}

cxBox4f cxNumberToBox4f(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeBox4f, "type error");
    return this->value.box4f;
}

cxAssist4f cxNumberToAssist4f(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeAssist4f, "type error");
    return this->value.assist4f;
}

cxColor4f cxNumberToColor4f(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeColor4f, "type error");
    return this->value.color4f;
}

cxVec2i cxNumberToVec2i(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeVec2i, "type error");
    return this->value.vec2i;
}

cxVec2f cxNumberToVec2f(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeVec2f, "type error");
    return this->value.vec2f;
}

cxSize2f cxNumberToSize2f(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeSize2f, "type error");
    return this->value.size2f;
}

cxInt cxNumberToInt(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeInt, "type error");
    return this->value.vi;
}

cxInt8 cxNumberToInt8(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeInt8, "type error");
    return this->value.i8;
}

cxInt16 cxNumberToInt16(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeInt16, "type error");
    return this->value.i16;
}

cxInt32 cxNumberToInt32(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeInt32, "type error");
    return this->value.i32;
}

cxInt64 cxNumberToInt64(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeInt64, "type error");
    return this->value.i64;
}

cxUInt cxNumberToUInt(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeUInt, "type error");
    return this->value.vu;
}

cxUInt8 cxNumberToUInt8(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeUInt8, "type error");
    return this->value.u8;
}

cxUInt16 cxNumberToUInt16(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeUInt16, "type error");
    return this->value.u16;
}

cxUInt32 cxNumberToUInt32(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeUInt32, "type error");
    return this->value.u32;
}

cxUInt64 cxNumberToUInt64(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeUInt64, "type error");
    return this->value.u64;
}

cxFloat cxNumberToFloat(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeFloat, "type error");
    return this->value.vf;
}

cxDouble cxNumberToDouble(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeDouble, "type error");
    return this->value.vd;
}

cxNumber cxNumberBool(cxBool v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeBool;
    rv->value.bv = v;
    return rv;
}

cxNumber cxNumberBox4i(cxBox4i v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeBox4i;
    rv->value.box4i = v;
    return rv;
}

cxNumber cxNumberVec3f(cxVec3f v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeVec3f;
    rv->value.vec3f = v;
    return rv;
}

cxNumber cxNumberVec2fRange(cxVec2fRange v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeVec2fRange;
    rv->value.vvr = v;
    return rv;
}

cxNumber cxNumberColor4fRange(cxColor4fRange v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeColor4fRange;
    rv->value.vcr = v;
    return rv;
}

cxNumber cxNumberFloatRange(cxFloatRange v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeFloatRange;
    rv->value.vfr = v;
    return rv;
}

cxNumber cxNumberBox4f(cxBox4f v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeBox4f;
    rv->value.box4f = v;
    return rv;
}

cxNumber cxNumberAssist4f(cxAssist4f v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeAssist4f;
    rv->value.assist4f = v;
    return rv;
}

cxNumber cxNumberVec2f(cxVec2f v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeVec2f;
    rv->value.vec2f = v;
    return rv;
}

cxNumber cxNumberVec2i(cxVec2i v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeVec2i;
    rv->value.vec2i = v;
    return rv;
}

cxNumber cxNumberSize2f(cxSize2f v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeSize2f;
    rv->value.size2f = v;
    return rv;
}

cxNumber cxNumberColor4f(cxColor4f v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeColor4f;
    rv->value.color4f = v;
    return rv;
}

cxNumber cxNumberInt(cxInt v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt;
    rv->value.vi = v;
    return rv;
}

cxNumber cxNumberInt8(cxInt8 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt8;
    rv->value.i8 = v;
    return rv;
}

cxNumber cxNumberInt16(cxInt16 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt16;
    rv->value.i16 = v;
    return rv;
}

cxNumber cxNumberInt32(cxInt32 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt32;
    rv->value.i32 = v;
    return rv;
}

cxNumber cxNumberInt64(cxInt64 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt64;
    rv->value.i64 = v;
    return rv;
}

cxNumber cxNumberUInt(cxUInt v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt;
    rv->value.vu = v;
    return rv;
}

cxNumber cxNumberUInt8(cxUInt8 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt8;
    rv->value.u8 = v;
    return rv;
}

cxNumber cxNumberUInt16(cxUInt16 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt16;
    rv->value.u16 = v;
    return rv;
}

cxNumber cxNumberUInt32(cxUInt32 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt32;
    rv->value.u32 = v;
    return rv;
}

cxNumber cxNumberUInt64(cxUInt64 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt64;
    rv->value.u64 = v;
    return rv;
}

cxNumber cxNumberFloat(cxFloat v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeFloat;
    rv->value.vf = v;
    return rv;
}

cxNumber cxNumberDouble(cxDouble v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeDouble;
    rv->value.vd = v;
    return rv;
}

