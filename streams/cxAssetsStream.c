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

static cxBool cxAssetsStreamOpen(cxAny this)
{
    cxAssetsStream asserts = this;
    CX_ASSERT(asserts->cxStream.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(asserts->cxStream.path);
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
    asserts->cxStream.length = (cxInt)stat.st_size;
    asserts->cxStream.canRead = true;
    asserts->cxStream.canSeek = true;
    asserts->cxStream.canWrite = false;
    asserts->cxStream.isOpen = true;
    return true;
}

static cxInt cxAssetsStreamRead(cxAny this,cxAny buffer,cxInt size)
{
    cxAssetsStream asserts = this;
    if(!asserts->cxStream.canRead){
        return 0;
    }
    return fread(buffer, 1, size, asserts->asset);
}

static cxInt cxAssetsStreamWrite(cxAny this,cxAny buffer,cxInt size)
{
    cxAssetsStream asserts = this;
    if(!asserts->cxStream.canWrite){
        return 0;
    }
    return 0;
}

static cxOff cxAssetsStreamPosition(cxAny this)
{
    cxAssetsStream asserts = this;
    if(!asserts->cxStream.canRead){
        return 0;
    }
    return (cxOff)ftell(asserts->asset);
}

static cxInt cxAssetsStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxAssetsStream asserts = this;
    if(!asserts->cxStream.canSeek){
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
    cxChars bytes = allocator->malloc(asserts->length + 1);
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

void __cxAssetsStreamInitType(cxAny type)
{
    
}

void __cxAssetsStreamInitObject(cxAny object,cxAny json,cxAny hash)
{
    
}

CX_OBJECT_TYPE(cxAssetsStream, cxStream)
{
    
}
CX_OBJECT_INIT(cxAssetsStream, cxStream)
{
    CX_METHOD_SET(this->cxStream.Open, cxAssetsStreamOpen);
    CX_METHOD_SET(this->cxStream.Read, cxAssetsStreamRead);
    CX_METHOD_SET(this->cxStream.Write, cxAssetsStreamWrite);
    CX_METHOD_SET(this->cxStream.Seek, cxAssetsStreamSeek);
    CX_METHOD_SET(this->cxStream.Close, cxAssetsStreamClose);
    CX_METHOD_SET(this->cxStream.Position,cxAssetsStreamPosition);
    CX_METHOD_SET(this->cxStream.AllBytes,cxAssetsStreamAllBytes);
}
CX_OBJECT_FREE(cxAssetsStream, cxStream)
{
    
}
CX_OBJECT_TERM(cxAssetsStream, cxStream)

cxString cxAssetsData(cxConstChars file)
{
    cxStream stream = cxAssetsStreamCreate(file);
    CX_RETURN(stream == NULL, NULL);
    return cxStreamAllBytes(stream);
}

cxBool cxAssetsStreamInit(cxAssetsStream this,cxConstChars file)
{
    if(cxDocumentExists(file)){
        CX_RETAIN_SWAP(this->cxStream.path, cxDocumentExists(file));
    }else if(cxAssetsExists(file)){
        CX_RETAIN_SWAP(this->cxStream.path, cxAssetsPath(file));
    }else{
        return false;
    }
    CX_RETAIN_SWAP(this->cxStream.file, cxStringConstChars(file));
    return true;
}

cxStream cxAssetsStreamCreate(cxConstChars file)
{
    cxAssetsStream rv = CX_CREATE(cxAssetsStream);
    if(!cxAssetsStreamInit(rv,file)){
        CX_ERROR("assets file %s create failed",file);
        return NULL;
    }
    return (cxStream)rv;
}







