//
//  cxStack.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxStack.h"

static cxInt cxStackLuaPush(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStack);
    cxObject any = CX_LUA_GET_PTR(2);
    cxStackPush(this, any);
    return 0;
}

static cxInt cxStackLuaPop(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStack);
    cxStackPop(this);
    return 0;
}

static cxInt cxStackLuaLength(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStack);
    lua_pushinteger(L, cxStackLength(this));
    return 1;
}

static cxInt cxStackLuaReplaceTop(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStack);
    cxObject any = CX_LUA_GET_PTR(2);
    cxStackReplaceTop(this, any);
    return 0;
}

static cxInt cxStackLuaTop(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStack);
    cxAny top = cxStackTop(this);
    CX_LUA_PUSH_OBJECT(top);
    return 1;
}

CX_LUA_METHOD_BEG(cxStack)
    {"Top",cxStackLuaTop},
    {"Push",cxStackLuaPush},
    {"Pop",cxStackLuaPop},
    {"Length",cxStackLuaLength},
    {"ReplaceTop",cxStackLuaReplaceTop},
CX_LUA_METHOD_END(cxStack)

void __cxStackTypeInit()
{
    CX_LUA_LOAD_TYPE(cxStack);
}

CX_OBJECT_INIT(cxStack, cxObject)
{
    this->array = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxStack, cxObject)
{
    CX_RELEASE(this->array);
}
CX_OBJECT_TERM(cxStack, cxObject)

void cxStackPush(cxStack stack,cxAny any)
{
    cxArrayAppend(stack->array, any);
}

cxAny cxStackTop(cxStack stack)
{
    return cxArrayLast(stack->array);
}

cxInt cxStackLength(cxStack stack)
{
    return cxArrayLength(stack->array);
}

void cxStackReplaceTop(cxStack stack,cxAny any)
{
    cxInt last = cxArrayLength(stack->array) - 1;
    if(last < 0){
        cxStackPush(stack, any);
    }else{
        cxArrayUpdate(stack->array, any, last);
    }
}

void cxStackPop(cxStack stack)
{
    CX_RETURN(cxArrayLength(stack->array) == 0);
    cxArrayRemoveLast(stack->array);
}

cxAny cxStackObject(cxStack stack,cxIndex index)
{
    return cxArrayAtIndex(stack->array, index);
}