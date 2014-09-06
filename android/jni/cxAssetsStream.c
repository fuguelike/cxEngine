//
//  cxAssetsStream.c
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <android/asset_manager.h>
#include <unistd.h>
#include <sys/stat.h>
#include <engine/cxUtil.h>
#include <streams/cxAssetsStream.h>
#include "cxAndroid.h"

static cxBool cxAssetsStreamOpen(cxAny this)
{
    cxAssetsStream asserts = this;
    CX_ASSERT(asserts->cxStream.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(asserts->cxStream.path);
    asserts->asset = AAssetManager_open(cxEngineGetAssetManager(), path, AASSET_MODE_UNKNOWN);
    if(asserts->asset == NULL){
        CX_ERROR("open asset file %s failed",path);
        return false;
    }
    asserts->cxStream.length = (cxInt)AAsset_getLength(asserts->asset);
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
    return AAsset_read(asserts->asset, buffer, size);
}

static cxInt cxAssetsStreamWrite(cxAny this,cxAny buffer,cxInt size)
{
    cxAssetsStream asserts = this;
    if(!asserts->cxStream.canWrite){
        CX_WARN("stream not support write");
        return 0;
    }
    return 0;
}

static cxOff cxAssetsStreamPosition(cxAny this)
{
    cxAssetsStream asserts = this;
    if(!asserts->cxStream.canRead){
        CX_WARN("stream not support read");
        return 0;
    }
    return (cxOff)AAsset_seek(asserts->asset, 0, SEEK_CUR);
}

static cxInt cxAssetsStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxAssetsStream asserts = this;
    if(!asserts->cxStream.canSeek){
        return false;
    }
    return AAsset_seek(asserts->asset, off, flags) > 0;
}

static cxString cxAssetsStreamAllBytes(cxAny this)
{
    cxStream asserts = this;
    if(!asserts->canRead){
        cxStreamOpen(asserts);
    }
    if(!asserts->canRead){
        CX_ERROR("file stream can't read");
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
        AAsset_close(asserts->asset);
        asserts->asset = NULL;
    }
    cxStreamBaseClose(this);
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
    cxAssetsStreamClose(this);
}
CX_OBJECT_TERM(cxAssetsStream, cxStream)

cxString cxAssetsData(cxConstChars file)
{
    cxStream stream = cxAssetsStreamCreate(file);
    return cxStreamAllBytes(stream);
}

cxStream cxAssetsStreamCreate(cxConstChars file)
{
    cxString path = cxAssetsPath(file);
    cxAssetsStream rv = CX_CREATE(cxAssetsStream);
    CX_RETAIN_SWAP(rv->cxStream.path, path);
    rv->cxStream.file = cxStringAllocChars(file);
    return (cxStream)rv;
}







