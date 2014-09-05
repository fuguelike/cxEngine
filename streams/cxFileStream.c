//
//  cxFileStream.c
//  cxEngine
//
//  Created by xuhua on 12/16/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <unistd.h>
#include <sys/stat.h>
#include <engine/cxUtil.h>
#include "cxFileStream.h"

static cxBool cxFileStreamOpen(cxAny this)
{
    cxFileStream file = this;
    CX_ASSERT(file->cxStream.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(file->cxStream.path);
    if(file->rdonly){
        file->fd = fopen(path, "rb");
    }else{
        file->fd = fopen(path, "wb");
    }
    if(file->fd == NULL){
        CX_ERROR("open file %s stream failed",path);
        return false;
    }
    struct stat stat={0};
    lstat(path, &stat);
    file->cxStream.length = (cxInt)stat.st_size;
    file->cxStream.canRead = true;
    file->cxStream.canSeek = true;
    file->cxStream.canWrite = true;
    file->cxStream.isOpen = true;
    return true;
}

static cxInt cxFileStreamRead(cxAny this,cxAny buffer,cxInt size)
{
    cxFileStream file = this;
    if(!file->cxStream.canRead){
        return 0;
    }
    return fread(buffer, 1, size, file->fd);
}

static cxInt cxFileStreamWrite(cxAny this,cxAny buffer,cxInt size)
{
    cxFileStream file = this;
    if(!file->cxStream.canWrite){
        return 0;
    }
    return fwrite(buffer, 1, size, file->fd);
}

static cxOff cxFileStreamPosition(cxAny this)
{
    cxFileStream file = this;
    if(!file->cxStream.canRead){
        return 0;
    }
    return (cxOff)ftell(file->fd);
}

static cxInt cxFileStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxFileStream file = this;
    if(!file->cxStream.canSeek){
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
    cxChars bytes = allocator->malloc(file->length + 1);
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

CX_OBJECT_TYPE(cxFileStream, cxStream)
{
    
}
CX_OBJECT_INIT(cxFileStream, cxStream)
{
    CX_METHOD_SET(this->cxStream.Open, cxFileStreamOpen);
    CX_METHOD_SET(this->cxStream.Read, cxFileStreamRead);
    CX_METHOD_SET(this->cxStream.Write, cxFileStreamWrite);
    CX_METHOD_SET(this->cxStream.Seek, cxFileStreamSeek);
    CX_METHOD_SET(this->cxStream.Close, cxFileStreamClose);
    CX_METHOD_SET(this->cxStream.Position,cxFileStreamPosition);
    CX_METHOD_SET(this->cxStream.AllBytes,cxFileStreamAllBytes);
}
CX_OBJECT_FREE(cxFileStream, cxStream)
{
    
}
CX_OBJECT_TERM(cxFileStream, cxStream)

cxString cxDocumentData(cxConstChars file)
{
    cxStream stream = cxFileStreamCreate(file, true);
    return cxStreamAllBytes(stream);
}

cxStream cxFileStreamCreate(cxConstChars file,cxBool rdonly)
{
    cxString path = cxDocumentPath(file);
    cxFileStream rv = CX_CREATE(cxFileStream);
    rv->rdonly = rdonly;
    CX_RETAIN_SWAP(rv->cxStream.path, path);
    rv->cxStream.file = cxStringAllocChars(file);
    return (cxStream)rv;
}








