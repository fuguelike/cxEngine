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

static cxBool cxFileStreamOpen(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxFileStream);
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(this->cxStream.path);
    if(this->rdonly){
        this->fd = fopen(path, "rb");
    }else{
        this->fd = fopen(path, "wb");
    }
    if(this->fd == NULL){
        CX_ERROR("open file %s stream failed",path);
        return false;
    }
    struct stat stat={0};
    lstat(path, &stat);
    this->cxStream.Length = (cxInt)stat.st_size;
    this->cxStream.canRead = true;
    this->cxStream.canSeek = true;
    this->cxStream.canWrite = true;
    this->cxStream.isOpen = true;
    return true;
}

static cxInt cxFileStreamRead(cxAny ps,cxAny buffer,cxInt size)
{
    CX_ASSERT_THIS(ps, cxFileStream);
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxInt)fread(buffer, 1, size, this->fd);
}

static cxInt cxFileStreamWrite(cxAny ps,cxAny buffer,cxInt size)
{
    CX_ASSERT_THIS(ps, cxFileStream);
    if(!this->cxStream.canWrite){
        return 0;
    }
    return (cxInt)fwrite(buffer, 1, size, this->fd);
}

static cxOff cxFileStreamPosition(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxFileStream);
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxOff)ftell(this->fd);
}

static cxInt cxFileStreamSeek(cxAny ps,cxOff off,cxInt flags)
{
    CX_ASSERT_THIS(ps, cxFileStream);
    if(!this->cxStream.canSeek){
        return false;
    }
    return fseek(this->fd, off, flags) > 0;
}

static cxString cxFileStreamAllBytes(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxStream);
    if(!this->canRead){
        cxStreamOpen(this);
    }
    if(!this->canRead){
        CX_ERROR("file stream can't read");
        return NULL;
    }
    cxStreamSeek(this,0,SEEK_END);
    this->Length = cxStreamPosition(this);
    cxStreamSeek(this,0,SEEK_SET);
    cxChars bytes = allocator->malloc(this->Length + 1);
    cxStreamRead(this,bytes,this->Length);
    bytes[this->Length] = '\0';
    cxString data = cxStringAttachMem(bytes, this->Length);
    cxStreamClose(this);
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

CX_TYPE(cxFileStream, cxStream)
{
    
}
CX_INIT(cxFileStream, cxStream)
{
    CX_SET(cxStream, this, Read, cxFileStreamRead);
    CX_SET(cxStream, this, Open, cxFileStreamOpen);
    CX_SET(cxStream, this, Write, cxFileStreamWrite);
    CX_SET(cxStream, this, Seek, cxFileStreamSeek);
    CX_SET(cxStream, this, Close, cxFileStreamClose);
    CX_SET(cxStream, this, Position, cxFileStreamPosition);
    CX_SET(cxStream, this, AllBytes, cxFileStreamAllBytes);
    
}
CX_FREE(cxFileStream, cxStream)
{
    
}
CX_TERM(cxFileStream, cxStream)

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
    return (cxStream)rv;
}








