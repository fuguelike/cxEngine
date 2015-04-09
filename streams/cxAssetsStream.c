//
//  cxAssetsStream.c
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <unistd.h>
#include <sys/stat.h>
#include <engine/cxUtil.h>
#include "cxAssetsStream.h"

CX_METHOD_DEF(cxAssetsStream,Open,cxBool)
{
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    cxConstChars path = cxStrBody(this->cxStream.path);
    this->asset = fopen(path, "rb");
    if(this->asset == NULL){
        CX_ERROR("open assets %s stream failed",path);
        return false;
    }
    struct stat stat={0};
    if(lstat(path, &stat) != 0){
        CX_ERROR("lstat assets %s stream failed",path);
        return false;
    }
    this->cxStream.Length = (cxInt)stat.st_size;
    this->cxStream.canRead = true;
    this->cxStream.canSeek = true;
    this->cxStream.canWrite = false;
    this->cxStream.isOpen = true;
    return true;
}
CX_METHOD_DEF(cxAssetsStream,Read,cxInt,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxInt)fread(buffer, 1, size, this->asset);
}
CX_METHOD_DEF(cxAssetsStream,Write,cxInt,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canWrite){
        return 0;
    }
    return 0;
}
CX_METHOD_DEF(cxAssetsStream,Position,cxInt)
{
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxInt)ftell(this->asset);
}

CX_METHOD_DEF(cxAssetsStream,Seek,cxInt,cxInt off,cxInt flags)
{
    if(!this->cxStream.canSeek){
        return false;
    }
    return fseek(this->asset, off, flags) > 0;
}
CX_METHOD_DEF(cxAssetsStream,AllBytes,cxStr)
{
    if(!this->cxStream.canRead){
        CX_CALL(this, Open, CX_M(void));
    }
    if(!this->cxStream.canRead){
        CX_ERROR("file %s stream can't read",cxStrBody(this->cxStream.path));
        return NULL;
    }
    CX_CALL(this, Seek, CX_M(cxInt,cxInt,cxInt),0,SEEK_SET);
    cxChars bytes = allocator->malloc(this->cxStream.Length + 1);
    CX_CALL(this, Read, CX_M(cxInt,cxAny,cxInt),bytes,this->cxStream.Length);
    bytes[this->cxStream.Length] = '\0';
    cxStr data = cxStrAttachMem(bytes, this->cxStream.Length);
    CX_CALL(this, Close, CX_M(void));
    return data;
}
CX_METHOD_DEF(cxAssetsStream,Close,void)
{
    cxAssetsStream asserts = this;
    if(asserts->asset != NULL){
        fclose(asserts->asset);
        asserts->asset = NULL;
    }
    CX_SUPER(cxStream, this, Close, CX_M(void));
}

CX_TYPE(cxAssetsStream, cxStream)
{
    CX_METHOD(cxAssetsStream, Open);
    CX_METHOD(cxAssetsStream, Read);
    CX_METHOD(cxAssetsStream, Write);
    CX_METHOD(cxAssetsStream, Seek);
    CX_METHOD(cxAssetsStream, Position);
    CX_METHOD(cxAssetsStream, Close);
    CX_METHOD(cxAssetsStream, AllBytes);
}
CX_INIT(cxAssetsStream, cxStream)
{

}
CX_FREE(cxAssetsStream, cxStream)
{
    
}
CX_TERM(cxAssetsStream, cxStream)

cxStr cxAssetsData(cxConstChars file)
{
    cxStream stream = cxAssetsStreamCreate(file);
    CX_RETURN(stream == NULL, NULL);
    return CX_CALL(stream, AllBytes, CX_M(cxStr));
}

cxStream cxAssetsStreamCreate(cxConstChars file)
{
    cxAssetsStream this = CX_CREATE(cxAssetsStream);
    CX_RETAIN_SWAP(this->cxStream.path, cxAssetsPath(file));
    return (cxStream)this;
}







