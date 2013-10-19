//
//  cxAssetsStream.c
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <sys/stat.h>
#include <core/cxUtil.h>
#include "cxAssetsStream.h"

static cxBool cxAssetsStreamOpen(cxAny this)
{
    cxAssetsStream asserts = this;
    CX_ASSERT(asserts->super.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(asserts->super.path);
    asserts->fd = fopen(path, "rb");
    if(asserts->fd == NULL){
        CX_ERROR("open assets %s stream failed",path);
        return false;
    }
    struct stat stat={0};
    lstat(path, &stat);
    asserts->super.length = (cxSize)stat.st_size;
    asserts->super.canRead = true;
    asserts->super.canSeek = true;
    asserts->super.canWrite = false;
    asserts->super.isOpen = true;
    return true;
}

static cxInt cxAssetsStreamRead(cxAny this,cxPointer buffer,cxInt size)
{
    cxAssetsStream asserts = this;
    if(!asserts->super.canRead){
        return 0;
    }
    int ret = (int)fread(buffer, 1, size, asserts->fd);
    return ret;
}

static cxInt cxAssetsStreamWrite(cxAny this,cxPointer buffer,cxInt size)
{
    cxAssetsStream asserts = this;
    if(!asserts->super.canWrite){
        return 0;
    }
    return 0;
}

static cxOff cxAssetsStreamPosition(cxAny this)
{
    cxAssetsStream asserts = this;
    if(!asserts->super.canRead){
        return 0;
    }
    return (cxOff)ftell(asserts->fd);
}

static cxBool cxAssetsStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxAssetsStream asserts = this;
    if(!asserts->super.canSeek){
        return false;
    }
    return fseek(asserts->fd, (long)off, flags);
}

static cxString cxAssetsStreamAllBytes(cxAny this)
{
    cxStream asserts = this;
    if(!asserts->canRead){
        asserts->interface->Open(asserts);
    }
    if(!asserts->canRead){
        CX_ERROR("file stream can't read");
        return NULL;
    }
    asserts->interface->Seek(asserts,0,SEEK_SET);
    cxChar *bytes = allocator->malloc(asserts->length);
    asserts->interface->Read(asserts,bytes,asserts->length);
    cxString data = cxStringAttach(bytes, asserts->length);
    asserts->interface->Close(asserts);
    return data;
}

static void cxAssetsStreamClose(cxAny this)
{
    cxAssetsStream asserts = this;
    if(asserts->fd != NULL){
        fclose(asserts->fd);
        asserts->fd = NULL;
    }
    cxStreamClose(this);
}

static const cxStreamInterface assetsInterface = {
    .Open       = cxAssetsStreamOpen,
    .Read       = cxAssetsStreamRead,
    .Write      = cxAssetsStreamWrite,
    .Seek       = cxAssetsStreamSeek,
    .Close      = cxAssetsStreamClose,
    .Position   = cxAssetsStreamPosition,
    .AllBytes   = cxAssetsStreamAllBytes,
};


CX_OBJECT_INIT(cxAssetsStream, cxStream)
{
    this->super.interface = &assetsInterface;
}
CX_OBJECT_FREE(cxAssetsStream, cxStream)
{
    cxAssetsStreamClose(this);
}
CX_OBJECT_TERM(cxAssetsStream, cxStream)

cxStream cxAssetsStreamCreate(cxConstChars file)
{
    cxString path = cxAssetsPath(file);
    cxAssetsStream rv = CX_CREATE(cxAssetsStream);
    CX_RETAIN_SWAP(rv->super.path, path);
    rv->super.file = CX_ALLOC(cxString);
    cxStringFormat(rv->super.file, "%s",file);
    return (cxStream)rv;
}







