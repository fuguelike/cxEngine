//
//  cxFileStream.c
//  cxEngine
//
//  Created by xuhua on 12/16/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <unistd.h>
#include <sys/stat.h>
#include <core/cxUtil.h>
#include "cxFileStream.h"

const luaL_Reg cxFileStreamInstanceMethods[] = {
    CX_LUA_SUPER(cxStream)
};

static cxInt cxFileStreamLuaCreate(lua_State *L)
{
    cxStream stream = cxFileStreamCreate(luaL_checkstring(L, 1));
    CX_LUA_PUSH_OBJECT(stream);
    return 1;
}

const luaL_Reg cxFileStreamTypeMethods[] = {
    {"create",cxFileStreamLuaCreate},
    {NULL,NULL}
};

void cxFileStreamTypeInit()
{
    CX_LUA_LOAD_TYPE(cxFileStream);
}

static cxBool cxFileStreamOpen(cxAny this)
{
    cxFileStream file = this;
    CX_ASSERT(file->super.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(file->super.path);
    file->fd = fopen(path, "wb");
    if(file->fd == NULL){
        CX_ERROR("open file %s stream failed",path);
        return false;
    }
    struct stat stat={0};
    lstat(path, &stat);
    file->super.length = (cxInt)stat.st_size;
    file->super.canRead = true;
    file->super.canSeek = true;
    file->super.canWrite = true;
    file->super.isOpen = true;
    return true;
}

static cxInt cxFileStreamRead(cxAny this,cxPointer buffer,cxInt size)
{
    cxFileStream file = this;
    if(!file->super.canRead){
        return 0;
    }
    return fread(buffer, 1, size, file->fd);
}

static cxInt cxFileStreamWrite(cxAny this,cxPointer buffer,cxInt size)
{
    cxFileStream file = this;
    if(!file->super.canWrite){
        return 0;
    }
    return fwrite(buffer, 1, size, file->fd);
}

static cxOff cxFileStreamPosition(cxAny this)
{
    cxFileStream file = this;
    if(!file->super.canRead){
        return 0;
    }
    return (cxOff)ftell(file->fd);
}

static cxInt cxFileStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxFileStream file = this;
    if(!file->super.canSeek){
        return false;
    }
    return fseek(file->fd, off, flags) > 0;
}

static cxString cxFileStreamAllBytes(cxAny this)
{
    cxStream file = this;
    if(!file->canRead){
        cxStreamOpen(file);
    }
    if(!file->canRead){
        CX_ERROR("file stream can't read");
        return NULL;
    }
    cxStreamSeek(file,0,SEEK_END);
    file->length = cxStreamPosition(file);
    cxStreamSeek(file,0,SEEK_SET);
    cxChar *bytes = allocator->malloc(file->length + 1);
    cxStreamRead(file,bytes,file->length);
    bytes[file->length] = '\0';
    cxString data = cxStringAttach(bytes, file->length);
    cxStreamClose(file);
    return data;
}

static void cxFileStreamClose(cxAny this)
{
    cxFileStream file = this;
    if(file->fd != NULL){
        fclose(file->fd);
        file->fd = NULL;
    }
    cxStreamBaseClose(this);
}

CX_OBJECT_INIT(cxFileStream, cxStream)
{
    CX_METHOD_OVERRIDE(this->super.Open, cxFileStreamOpen);
    CX_METHOD_OVERRIDE(this->super.Read, cxFileStreamRead);
    CX_METHOD_OVERRIDE(this->super.Write, cxFileStreamWrite);
    CX_METHOD_OVERRIDE(this->super.Seek, cxFileStreamSeek);
    CX_METHOD_OVERRIDE(this->super.Close, cxFileStreamClose);
    CX_METHOD_OVERRIDE(this->super.Position,cxFileStreamPosition);
    CX_METHOD_OVERRIDE(this->super.AllBytes,cxFileStreamAllBytes);
}
CX_OBJECT_FREE(cxFileStream, cxStream)
{
    cxFileStreamClose(this);
}
CX_OBJECT_TERM(cxFileStream, cxStream)

cxStream cxFileStreamCreate(cxConstChars file)
{
    cxString path = cxDocumentPath(file);
    cxFileStream rv = CX_CREATE(cxFileStream);
    CX_RETAIN_SWAP(rv->super.path, path);
    rv->super.file = cxStringAllocChars(file);
    return (cxStream)rv;
}








