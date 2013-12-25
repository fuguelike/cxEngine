//
//  cxMemStream.c
//  cxEngine
//
//  Created by xuhua on 9/30/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxMemStream.h"

const luaL_Reg cxMemStreamInstanceMethods[] = {
    CX_LUA_SUPER(cxStream)
};

static cxInt cxMemStreamLuaCreate(lua_State *L)
{
    cxStream stream = CX_CREATE(cxMemStream);
    CX_LUA_PUSH_OBJECT(stream);
    return 1;
}

const luaL_Reg cxMemStreamTypeMethods[] = {
    {"create",cxMemStreamLuaCreate},
    {NULL,NULL}
};

void cxMemStreamTypeInit()
{
    CX_LUA_LOAD_TYPE(cxMemStream);
}

static cxBool cxMemStreamOpen(cxAny this)
{
    cxMemStream stream = this;
    CX_ASSERT(stream->super.isOpen == false,"stream repeat open");
    stream->allocSize = 128;
    stream->data = allocator->malloc(stream->allocSize);
    stream->super.canRead = true;
    stream->super.canWrite = true;
    stream->super.canSeek = true;
    stream->super.isOpen = true;
    return true;
}

static cxInt cxMemStreamRead(cxAny this,cxPointer buffer,cxInt size)
{
    cxMemStream stream = this;
    if(!stream->super.canRead){
        return 0;
    }
    cxInt bytes = (cxInt)(stream->super.length - stream->position);
    if(bytes <= 0){
        return 0;
    }
    if(bytes > size){
        bytes = size;
    }
    memcpy(buffer, stream->data + stream->position, bytes);
    stream->position += bytes;
    return bytes;
}

static cxInt cxMemStreamWrite(cxAny this,cxPointer buffer,cxInt size)
{
    cxMemStream stream = this;
    if(!stream->super.canWrite){
        return 0;
    }
    cxInt bytes = (cxInt)(stream->allocSize - stream->position);
    if(bytes < size){
        stream->allocSize += (size + 128);
        stream->data = allocator->realloc(stream->data,stream->allocSize);
        CX_ASSERT(stream->data != NULL, "memory realloc failed");
    }
    memcpy(stream->data + stream->position, buffer, size);
    stream->position += size;
    stream->super.length += size;
    return size;
}

static cxOff cxMemStreamPosition(cxAny this)
{
    cxMemStream stream = this;
    return stream->position;
}

static cxInt cxMemStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxMemStream stream = this;
    if(!stream->super.canSeek){
        return 0;
    }
    if(flags == SEEK_SET && off < stream->super.length){
        stream->position = off;
        return stream->position;
    }
    cxInt seek = (cxInt)(stream->super.length - stream->position);
    if(flags == SEEK_CUR && off < seek){
        stream->position += off;
        return stream->position;
    }
    seek = (cxInt)(stream->super.length + off);
    if(flags == SEEK_END && seek < stream->super.length){
        stream->position = seek;
        return stream->position;
    }
    return 0;
}

static cxString cxMemStreamAllBytes(cxAny this)
{
    cxMemStream stream = this;
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, stream->data, stream->super.length);
    return rv;
}

static void cxMemStreamClose(cxAny this)
{
    cxMemStream stream = this;
    stream->allocSize = 0;
    allocator->free(stream->data);
    stream->data = NULL;
    stream->position = 0;
    cxStreamBaseClose(this);
}

CX_OBJECT_INIT(cxMemStream, cxStream)
{
    CX_METHOD_OVERRIDE(this->super.Open, cxMemStreamOpen);
    CX_METHOD_OVERRIDE(this->super.Read, cxMemStreamRead);
    CX_METHOD_OVERRIDE(this->super.Write, cxMemStreamWrite);
    CX_METHOD_OVERRIDE(this->super.Seek, cxMemStreamSeek);
    CX_METHOD_OVERRIDE(this->super.Close, cxMemStreamClose);
    CX_METHOD_OVERRIDE(this->super.Position,cxMemStreamPosition);
    CX_METHOD_OVERRIDE(this->super.AllBytes,cxMemStreamAllBytes);
}
CX_OBJECT_FREE(cxMemStream, cxStream)
{
    allocator->free(this->data);
    this->data = NULL;
}
CX_OBJECT_TERM(cxMemStream, cxStream)

cxStream cxMemStreamCreateWithText(cxString txt)
{
    cxStream stream = CX_CREATE(cxMemStream);
    if(!cxStreamOpen(stream)){
        return NULL;
    }
    cxStreamWrite(stream,(cxPointer)cxStringBody(txt),cxStringLength(txt));
    return stream;
}










