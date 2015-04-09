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

CX_METHOD_DEF(cxFileStream,Open,cxBool)
{
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    cxConstChars path = cxStrBody(this->cxStream.path);
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
CX_METHOD_DEF(cxFileStream,Read,cxInt,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxInt)fread(buffer, 1, size, this->fd);
}
CX_METHOD_DEF(cxFileStream,Write,cxInt,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canWrite){
        return 0;
    }
    return (cxInt)fwrite(buffer, 1, size, this->fd);
}
CX_METHOD_DEF(cxFileStream,Position,cxInt)
{
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxInt)ftell(this->fd);
}
CX_METHOD_DEF(cxFileStream,Seek,cxInt,cxInt off,cxInt flags)
{
    if(!this->cxStream.canSeek){
        return false;
    }
    return fseek(this->fd, off, flags) > 0;
}
CX_METHOD_DEF(cxFileStream,AllBytes,cxStr)
{
    if(!this->cxStream.canRead){
        CX_CALL(this, Open, CX_M(cxBool));
    }
    if(!this->cxStream.canRead){
        return NULL;
    }
    CX_CALL(this, Seek, CX_M(cxInt,cxInt,cxInt),0,SEEK_END);
    this->cxStream.Length = CX_CALL(this, Position, CX_M(cxInt));
    CX_CALL(this, Seek, CX_M(cxInt,cxInt,cxInt),0,SEEK_SET);
    cxChars bytes = allocator->malloc(this->cxStream.Length + 1);
    CX_CALL(this, Read, CX_M(cxInt,cxAny,cxInt),bytes,this->cxStream.Length);
    bytes[this->cxStream.Length] = '\0';
    cxStr data = cxStrAttachMem(bytes, this->cxStream.Length);
    CX_CALL(this, Close, CX_M(void));
    return data;
}
CX_METHOD_DEF(cxFileStream,Close,void)
{
    if(this->fd != NULL){
        fclose(this->fd);
        this->fd = NULL;
    }
    CX_SUPER(cxStream, this, Close, CX_M(void));
}

CX_TYPE(cxFileStream, cxStream)
{
    CX_METHOD(cxFileStream, Open);
    CX_METHOD(cxFileStream, Read);
    CX_METHOD(cxFileStream, Write);
    CX_METHOD(cxFileStream, Seek);
    CX_METHOD(cxFileStream, Position);
    CX_METHOD(cxFileStream, Close);
    CX_METHOD(cxFileStream, AllBytes);
}
CX_INIT(cxFileStream, cxStream)
{
    
    
}
CX_FREE(cxFileStream, cxStream)
{
    
}
CX_TERM(cxFileStream, cxStream)

cxStr cxDocumentData(cxConstChars file)
{
    cxStream stream = cxFileStreamCreate(file, true);
    return CX_CALL(stream, AllBytes, CX_M(cxStr));
}

cxStream cxFileStreamCreate(cxConstChars file,cxBool rdonly)
{
    cxStr path = cxDocumentPath(file);
    cxFileStream rv = CX_CREATE(cxFileStream);
    rv->rdonly = rdonly;
    CX_RETAIN_SWAP(rv->cxStream.path, path);
    return (cxStream)rv;
}








