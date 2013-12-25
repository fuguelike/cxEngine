//
//  cxHash.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxString.h"
#include "cxNumber.h"
#include "cxHash.h"

static cxInt cxHashLuaGet(lua_State *L)
{
    CX_LUA_DEF_THIS(cxHash);
    cxConstChars key = lua_tostring(L, 2);
    cxAny any = cxHashGet(this, cxHashStrKey(key));
    CX_LUA_PUSH_OBJECT(any);
    return 1;
}

static cxInt cxHashLuaSet(lua_State *L)
{
    CX_LUA_DEF_THIS(cxHash);
    cxAny any = cxLuaObjectValue(L, 3);
    if(any == NULL){
        luaL_error(L, "cxHash:set(int|string,any) any args error");
        return 0;
    }
    cxConstChars key = lua_tostring(L, 2);
    cxHashSet(this, cxHashStrKey(key), any);
    return 0;
}

static cxInt cxHashLuaDel(lua_State *L)
{
    CX_LUA_DEF_THIS(cxHash);
    cxConstChars key = lua_tostring(L, 2);
    cxHashDel(this, cxHashStrKey(key));
    return 0;
}

static cxInt cxHashLuaEach(lua_State *L)
{
    CX_LUA_DEF_THIS(cxHash);
    if(!lua_isfunction(L, 2)){
        luaL_error(L, "each function error");
        return 0;
    }
    CX_HASH_FOREACH(this, ele, tmp){
        lua_pushvalue(L, 2);
        lua_pushstring(L, ele->key);
        CX_LUA_PUSH_OBJECT(ele->any);
        lua_call(L, 2, 0);
    }
    return 0;
}

static cxInt cxHashLuaLength(lua_State *L)
{
    CX_LUA_DEF_THIS(cxHash);
    lua_pushnumber(L, cxHashLength(this));
    return 1;
}

static cxInt cxHashLuaClean(lua_State *L)
{
    CX_LUA_DEF_THIS(cxHash);
    cxHashClean(this);
    return 0;
}

const luaL_Reg cxHashInstanceMethods[] = {
    {"length",cxHashLuaLength},
    {"get",cxHashLuaGet},
    {"set",cxHashLuaSet},
    {"del",cxHashLuaDel},
    {"each",cxHashLuaEach},
    {"clean",cxHashLuaClean},
    CX_LUA_SUPER(cxObject)
};

const luaL_Reg cxHashTypeMethods[] = {
    CX_LUA_TYPE(cxHash)
};

void cxHashTypeInit()
{
    CX_LUA_LOAD_TYPE(cxHash);
}

CX_OBJECT_INIT(cxHash, cxObject)
{
    
}
CX_OBJECT_FREE(cxHash, cxObject)
{
    cxHashClean(this);
}
CX_OBJECT_TERM(cxHash, cxObject)

void cxHashClean(cxHash hash)
{
    cxHashElement *ele = NULL, *tmp = NULL;
    HASH_ITER(hh, hash->hashPtr, ele, tmp){
        CX_RELEASE(ele->any);
    }
    HASH_CLEAR(hh, hash->hashPtr);
}

void cxHashDelElement(cxHash hash,cxHashElement *element)
{
    HASH_DEL(hash->hashPtr, element);
    CX_RELEASE(element->any);
    allocator->free(element);
}

cxBool cxHashHas(cxHash hash,cxHashKey key)
{
    CX_ASSERT(key.data != NULL && key.length > 0, "key keySize error");
    CX_ASSERT(key.length < CX_HASH_MAX_KEY_LENGTH, "keySize too longer");
    cxHashElement *element = NULL;
    HASH_FIND(hh, hash->hashPtr, key.data, key.length, element);
    return element != NULL;
}

void cxHashDel(cxHash hash,cxHashKey key)
{
    cxHashElement *element = NULL;
    HASH_FIND(hh, hash->hashPtr, key.data, key.length, element);
    if(element != NULL){
        cxHashDelElement(hash, element);
    }
}

cxInt cxHashLength(cxHash hash)
{
    return HASH_COUNT(hash->hashPtr);
}

cxAny cxHashFirst(cxHash hash)
{
    cxHashElement *ele = NULL, *tmp = NULL;
    HASH_ITER(hh, hash->hashPtr, ele, tmp){
        return ele->any;
    }
    return NULL;
}

cxAny cxHashGet(cxHash hash,cxHashKey key)
{
    cxHashElement *element = cxHashGetElement(hash, key);
    return element == NULL ? NULL : element->any;
}

cxHashElement *cxHashGetElement(cxHash hash,cxHashKey key)
{
    CX_ASSERT(key.data != NULL && key.length > 0, "key keySize error");
    CX_ASSERT(key.length < CX_HASH_MAX_KEY_LENGTH, "keySize too longer");
    cxHashElement *element = NULL;
    HASH_FIND(hh, hash->hashPtr, key.data, key.length, element);
    return element;
}

static void cxHashSetUnsafe(cxHash hash,cxHashKey key,cxAny any)
{
    cxHashElement *new = allocator->malloc(sizeof(cxHashElement));
    memcpy(new->key, key.data, key.length);
    new->key[key.length] = '\0';
    new->any = any;
    HASH_ADD(hh, hash->hashPtr, key, key.length, new);
    CX_RETAIN(new->any);
}

void cxHashSet(cxHash hash,cxHashKey key,cxAny any)
{
    CX_ASSERT(any != NULL, "any object error");
    cxHashElement *element = cxHashGetElement(hash, key);
    if(element == NULL){
        cxHashSetUnsafe(hash, key, any);
    }else if(element->any != any){
        CX_RETAIN_SWAP(element->any, any);
    }
}











