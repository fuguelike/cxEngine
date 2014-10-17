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

static cxBool cxAssetsStreamOpen(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxAssetsStream);
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

static cxInt cxAssetsStreamRead(cxAny ps,cxAny buffer,cxInt size)
{
    CX_ASSERT_THIS(ps, cxAssetsStream);
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxInt)fread(buffer, 1, size, this->asset);
}

static cxInt cxAssetsStreamWrite(cxAny ps,cxAny buffer,cxInt size)
{
    CX_ASSERT_THIS(ps, cxAssetsStream);
    if(!this->cxStream.canWrite){
        return 0;
    }
    return 0;
}

static cxOff cxAssetsStreamPosition(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxAssetsStream);
    if(!this->cxStream.canRead){
        return 0;
    }
    return (cxOff)ftell(this->asset);
}

static cxInt cxAssetsStreamSeek(cxAny ps,cxOff off,cxInt flags)
{
    CX_ASSERT_THIS(ps, cxAssetsStream);
    if(!this->cxStream.canSeek){
        return false;
    }
    return fseek(this->asset, off, flags) > 0;
}

static cxString cxAssetsStreamAllBytes(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxStream);
    if(!this->canRead){
        cxStreamOpen(this);
    }
    if(!this->canRead){
        CX_ERROR("file %s stream can't read",cxStringBody(this->path));
        return NULL;
    }
    cxStreamSeek(this,0,SEEK_SET);
    cxChars bytes = allocator->malloc(this->Length + 1);
    cxStreamRead(this,bytes,this->Length);
    bytes[this->Length] = '\0';
    cxString data = cxStringAttachMem(bytes, this->Length);
    cxStreamClose(this);
    return data;
}

static void cxAssetsStreamClose(cxAny this)
{
    cxAssetsStream asserts = this;
    if(asserts->asset != NULL){
        fclose(asserts->asset);
        asserts->asset = NULL;
    }
    cxStreamBaseClose(this);
}

CX_TYPE(cxAssetsStream, cxStream)
{
    
}
CX_INIT(cxAssetsStream, cxStream)
{
    CX_SET(cxStream, this, Read, cxAssetsStreamRead);
    CX_SET(cxStream, this, Open, cxAssetsStreamOpen);
    CX_SET(cxStream, this, Write, cxAssetsStreamWrite);
    CX_SET(cxStream, this, Seek, cxAssetsStreamSeek);
    CX_SET(cxStream, this, Close, cxAssetsStreamClose);
    CX_SET(cxStream, this, Position, cxAssetsStreamPosition);
    CX_SET(cxStream, this, AllBytes, cxAssetsStreamAllBytes);
}
CX_FREE(cxAssetsStream, cxStream)
{
    
}
CX_TERM(cxAssetsStream, cxStream)

cxString cxAssetsData(cxConstChars file)
{
    cxStream stream = cxAssetsStreamCreate(file);
    CX_RETURN(stream == NULL, NULL);
    return cxStreamAllBytes(stream);
}

cxStream cxAssetsStreamCreate(cxConstChars file)
{
    cxAssetsStream this = CX_CREATE(cxAssetsStream);
    CX_RETAIN_SWAP(this->cxStream.path, cxAssetsPath(file));
    return (cxStream)this;
}







