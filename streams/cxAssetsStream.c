//
//  cxAssetsStream.c
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <unistd.h>
#include <sys/stat.h>
#include <core/cxUtil.h>
#include "cxAssetsStream.h"
#include "cxAssetsLuaImp.c"

static cxBool cxAssetsStreamOpen(cxAny this)
{
    cxAssetsStream asserts = this;
    CX_ASSERT(asserts->super.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(asserts->super.path);
    asserts->asset = fopen(path, "rb");
    if(asserts->asset == NULL){
        CX_ERROR("open assets %s stream failed",path);
        return false;
    }
    struct stat stat={0};
    if(lstat(path, &stat) != 0){
        CX_ERROR("lstat assets %s stream failed",path);
        return false;
    }
    asserts->super.length = (cxInt)stat.st_size;
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
    return fread(buffer, 1, size, asserts->asset);
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
    return (cxOff)ftell(asserts->asset);
}

static cxInt cxAssetsStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxAssetsStream asserts = this;
    if(!asserts->super.canSeek){
        return false;
    }
    return fseek(asserts->asset, off, flags) > 0;
}

static cxString cxAssetsStreamAllBytes(cxAny this)
{
    cxStream asserts = this;
    if(!asserts->canRead){
        cxStreamOpen(asserts);
    }
    if(!asserts->canRead){
        CX_ERROR("file %s stream can't read",cxStringBody(asserts->file));
        return NULL;
    }
    cxStreamSeek(asserts,0,SEEK_SET);
    cxChar *bytes = allocator->malloc(asserts->length + 1);
    cxStreamRead(asserts,bytes,asserts->length);
    bytes[asserts->length] = '\0';
    cxString data = cxStringAttach(bytes, asserts->length);
    cxStreamClose(asserts);
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

CX_OBJECT_INIT(cxAssetsStream, cxStream)
{
    CX_METHOD_OVERRIDE(this->super.Open, cxAssetsStreamOpen);
    CX_METHOD_OVERRIDE(this->super.Read, cxAssetsStreamRead);
    CX_METHOD_OVERRIDE(this->super.Write, cxAssetsStreamWrite);
    CX_METHOD_OVERRIDE(this->super.Seek, cxAssetsStreamSeek);
    CX_METHOD_OVERRIDE(this->super.Close, cxAssetsStreamClose);
    CX_METHOD_OVERRIDE(this->super.Position,cxAssetsStreamPosition);
    CX_METHOD_OVERRIDE(this->super.AllBytes,cxAssetsStreamAllBytes);
}
CX_OBJECT_FREE(cxAssetsStream, cxStream)
{
    cxAssetsStreamClose(this);
}
CX_OBJECT_TERM(cxAssetsStream, cxStream)

cxString cxAssetsData(cxConstChars file)
{
    cxStream stream = cxAssetsStreamCreate(file);
    return cxStreamAllBytes(stream);
}

void cxAssetsStreamInit(cxAssetsStream this,cxConstChars file)
{
    CX_RETAIN_SWAP(this->super.path, cxAssetsPath(file));
    CX_RETAIN_SWAP(this->super.file, cxStringConstChars(file));
}

cxStream cxAssetsStreamCreate(cxConstChars file)
{
    cxAssetsStream rv = CX_CREATE(cxAssetsStream);
    cxAssetsStreamInit(rv,file);
    return (cxStream)rv;
}







