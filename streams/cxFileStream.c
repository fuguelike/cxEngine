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

static cxBool CX_METHOD(cxFileStream,Open)
{
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(this->cxStream.path);
    if(this->rdonly){
        this->fd = fopen(path, "rb");
    }else{
        this->fd = fopen(path, "wb");
    }
    if(this->fd == NULL){
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

static cxInt CX_METHOD(cxFileStream,Read,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxInt)fread(buffer, 1, size, this->fd);
}

static cxInt CX_METHOD(cxFileStream,Write,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canWrite){
        return 0;
    }
    return (cxInt)fwrite(buffer, 1, size, this->fd);
}

static cxOff CX_METHOD(cxFileStream,Position)
{
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxOff)ftell(this->fd);
}

static cxInt CX_METHOD(cxFileStream,Seek,cxOff off,cxInt flags)
{
    if(!this->cxStream.canSeek){
        return false;
    }
    return fseek(this->fd, off, flags) > 0;
}

static cxString CX_METHOD(cxFileStream,AllBytes)
{
    if(!this->cxStream.canRead){
        CX_CALL(this, Open, CX_MT(cxBool));
    }
    if(!this->cxStream.canRead){
        return NULL;
    }
    CX_CALL(this, Seek, CX_MT(cxInt,cxOff,cxInt),0,SEEK_END);
    this->cxStream.Length = CX_CALL(this, Position, CX_MT(cxInt));
    CX_CALL(this, Seek, CX_MT(cxInt,cxOff,cxInt),0,SEEK_SET);
    cxChars bytes = allocator->malloc(this->cxStream.Length + 1);
    CX_CALL(this, Read, CX_MT(cxInt,cxAny,cxInt),bytes,this->cxStream.Length);
    bytes[this->cxStream.Length] = '\0';
    cxString data = cxStringAttachMem(bytes, this->cxStream.Length);
    CX_CALL(this, Close, CX_MT(void));
    return data;
}

static void CX_METHOD(cxFileStream,Close)
{
    if(this->fd != NULL){
        fclose(this->fd);
        this->fd = NULL;
    }
    CX_SUPER(cxStream, this, Close, CX_MT(void));
}

CX_TYPE(cxFileStream, cxStream)
{
    CX_MSET(cxFileStream, Open);
    CX_MSET(cxFileStream, Read);
    CX_MSET(cxFileStream, Write);
    CX_MSET(cxFileStream, Seek);
    CX_MSET(cxFileStream, Position);
    CX_MSET(cxFileStream, Close);
    CX_MSET(cxFileStream, AllBytes);
}
CX_INIT(cxFileStream, cxStream)
{
    
    
}
CX_FREE(cxFileStream, cxStream)
{
    
}
CX_TERM(cxFileStream, cxStream)

cxString cxDocumentData(cxConstChars file)
{
    cxStream stream = cxFileStreamCreate(file, true);
    return CX_CALL(stream, AllBytes, CX_MT(cxString));
}

cxStream cxFileStreamCreate(cxConstChars file,cxBool rdonly)
{
    cxString path = cxDocumentPath(file);
    cxFileStream rv = CX_CREATE(cxFileStream);
    rv->rdonly = rdonly;
    CX_RETAIN_SWAP(rv->cxStream.path, path);
    return (cxStream)rv;
}








