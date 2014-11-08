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

static cxBool CX_METHOD(cxAssetsStream,Open)
{
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(this->cxStream.path);
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

static cxInt CX_METHOD(cxAssetsStream,Read,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxInt)fread(buffer, 1, size, this->asset);
}

static cxInt CX_METHOD(cxAssetsStream,Write,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canWrite){
        return 0;
    }
    return 0;
}

static cxOff CX_METHOD(cxAssetsStream,Position)
{
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxOff)ftell(this->asset);
}

static cxInt CX_METHOD(cxAssetsStream,Seek,cxOff off,cxInt flags)
{
    if(!this->cxStream.canSeek){
        return false;
    }
    return fseek(this->asset, off, flags) > 0;
}

static cxString CX_METHOD(cxAssetsStream,AllBytes)
{
    if(!this->cxStream.canRead){
        CX_CALL(this, Open, CX_MT(void));
    }
    if(!this->cxStream.canRead){
        CX_ERROR("file %s stream can't read",cxStringBody(this->cxStream.path));
        return NULL;
    }
    CX_CALL(this, Seek, CX_MT(cxInt,cxOff,cxInt),0,SEEK_SET);
    cxChars bytes = allocator->malloc(this->cxStream.Length + 1);
    CX_CALL(this, Read, CX_MT(cxInt,cxAny,cxInt),bytes,this->cxStream.Length);
    bytes[this->cxStream.Length] = '\0';
    cxString data = cxStringAttachMem(bytes, this->cxStream.Length);
    CX_CALL(this, Close, CX_MT(void));
    return data;
}

static void cxAssetsStreamClose(cxAny this)
{
    cxAssetsStream asserts = this;
    if(asserts->asset != NULL){
        fclose(asserts->asset);
        asserts->asset = NULL;
    }
    CX_SUPER(cxStream, this, Close, CX_MT(void));
}

CX_TYPE(cxAssetsStream, cxStream)
{
    CX_MSET(cxAssetsStream, Open);
    CX_MSET(cxAssetsStream, Read);
    CX_MSET(cxAssetsStream, Write);
    CX_MSET(cxAssetsStream, Seek);
    CX_MSET(cxAssetsStream, Position);
    CX_MSET(cxAssetsStream, Close);
    CX_MSET(cxAssetsStream, AllBytes);
}
CX_INIT(cxAssetsStream, cxStream)
{

}
CX_FREE(cxAssetsStream, cxStream)
{
    CX_CALL(this, Close, CX_MT(void));
}
CX_TERM(cxAssetsStream, cxStream)

cxString cxAssetsData(cxConstChars file)
{
    cxStream stream = cxAssetsStreamCreate(file);
    CX_RETURN(stream == NULL, NULL);
    return CX_CALL(stream, AllBytes, CX_MT(cxString));
}

cxStream cxAssetsStreamCreate(cxConstChars file)
{
    cxAssetsStream this = CX_CREATE(cxAssetsStream);
    CX_RETAIN_SWAP(this->cxStream.path, cxAssetsPath(file));
    return (cxStream)this;
}







