//
//  cxStream.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <libgen.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxMemStream.h>
#include <streams/cxFileStream.h>
#include "cxTypes.h"
#include "cxStream.h"


static cxInt cxStreamLuaOpen(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStream);
    cxBool ret = cxStreamOpen(this);
    lua_pushboolean(L, ret);
    return 1;
}

static cxInt cxStreamLuaRead(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStream);
    cxInt size = cxLuaIntValue(L, 2, 0);
    if(size <= 0){
        luaL_error(L, "size args error");
        return 0;
    }
    cxPointer buffer = allocator->malloc(size);
    cxInt bytes = cxStreamRead(this, buffer, size);
    cxString rv = cxStringAttach(buffer, bytes);
    CX_LUA_PUSH_OBJECT(rv);
    return 1;
}

static cxInt cxStreamLuaWrite(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStream);
    size_t len = 0;
    cxInt rv = 0;
    cxConstChars buffer = lua_tolstring(L, 2, &len);
    if(len > 0){
        rv = cxStreamWrite(this, (cxPointer)buffer, len);
    }
    lua_pushinteger(L, rv);
    return 1;
}

static cxInt cxStreamLuaSeek(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStream);
    cxInt off = cxLuaIntValue(L, 2, 0);
    cxInt flag = cxLuaIntValue(L, 3, SEEK_SET);
    off = cxStreamSeek(this, off, flag);
    lua_pushinteger(L, off);
    return 1;
}

static cxInt cxStreamLuaClose(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStream);
    cxStreamClose(this);
    return 0;
}

static cxInt cxStreamLuaPosition(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStream);
    lua_pushinteger(L, cxStreamPosition(this));
    return 1;
}

static cxInt cxStreamLuaAllBytes(lua_State *L)
{
    CX_LUA_DEF_THIS(cxStream);
    cxString bytes = cxStreamAllBytes(this);
    CX_LUA_PUSH_OBJECT(bytes);
    return 1;
}

static cxInt cxStreamLuaMakeAssets(lua_State *L)
{
    cxConstChars file = luaL_checkstring(L, 1);
    cxStream this = cxAssetsStreamCreate(file);
    CX_LUA_PUSH_OBJECT(this);
    return 1;
}

static cxInt cxStreamLuaMakeMemory(lua_State *L)
{
    cxStream this = cxMemStreamCreateWithText(NULL);
    CX_LUA_PUSH_OBJECT(this);
    return 1;
}

static cxInt cxStreamLuaMakeFile(lua_State *L)
{
    cxConstChars file = luaL_checkstring(L, 1);
    cxStream this = cxFileStreamCreate(file);
    CX_LUA_PUSH_OBJECT(this);
    return 1;
}

CX_LUA_METHOD_BEGIN(cxStream)
    {"open",cxStreamLuaOpen},
    {"read",cxStreamLuaRead},
    {"write",cxStreamLuaWrite},
    {"seek",cxStreamLuaSeek},
    {"close",cxStreamLuaClose},
    {"position",cxStreamLuaPosition},
    {"allBytes",cxStreamLuaAllBytes},
    {"assets",cxStreamLuaMakeAssets},
    {"memory",cxStreamLuaMakeMemory},
    {"file",cxStreamLuaMakeFile},
CX_LUA_METHOD_END(cxStream)

void cxStreamTypeInit()
{
    CX_LUA_LOAD_TYPE(cxStream);
}

CX_OBJECT_INIT(cxStream, cxObject)
{
}
CX_OBJECT_FREE(cxStream, cxObject)
{
    if(this->isOpen){
        cxStreamClose(this);
    }
    CX_RELEASE(this->path);
    CX_RELEASE(this->file);
    CX_METHOD_RELEASE(this->Read);
    CX_METHOD_RELEASE(this->Write);
    CX_METHOD_RELEASE(this->Open);
    CX_METHOD_RELEASE(this->Seek);
    CX_METHOD_RELEASE(this->AllBytes);
    CX_METHOD_RELEASE(this->Close);
    CX_METHOD_RELEASE(this->Position);
}
CX_OBJECT_TERM(cxStream, cxObject)

cxBool cxStreamOpen(cxAny this)
{
    cxStream stream = this;
    return CX_METHOD_FIRE(false, stream->Open, this);
}

cxInt cxStreamRead(cxAny this,cxPointer buffer,cxInt size)
{
    cxStream stream = this;
    return CX_METHOD_FIRE(0, stream->Read,this,buffer,size);
}

cxInt cxStreamWrite(cxAny this,cxPointer buffer,cxInt size)
{
    cxStream stream = this;
    return CX_METHOD_FIRE(0, stream->Write,this,buffer,size);
}

cxInt cxStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxStream stream = this;
    return CX_METHOD_FIRE(0, stream->Seek,this,off,flags);
}

cxOff cxStreamPosition(cxAny this)
{
    cxStream stream = this;
    return CX_METHOD_FIRE(0, stream->Position, this);
}

cxInt cxStreamLength(cxAny this)
{
    cxStream stream = this;
    return stream->length;
}

void cxStreamClose(cxAny this)
{
    cxStream stream = this;
    CX_METHOD_FIRE(NULL, stream->Close,this);
}

cxString cxStreamAllBytes(cxAny this)
{
    cxStream stream = this;
    return CX_METHOD_FIRE(NULL, stream->AllBytes,this);
}

void cxStreamBaseClose(cxAny this)
{
    cxStream stream = this;
    stream->length = 0;
    stream->canRead = false;
    stream->canWrite = false;
    stream->canSeek = false;
    stream->isOpen = false;
}

cxString cxStreamFileDir(cxAny this)
{
    cxStream stream = this;
    if(stream->path == NULL){
        return cxStringCreate(".");
    }
    char *dir = dirname((char *)cxStringBody(stream->file));
    return cxStringCreate("%s",dir);
}












