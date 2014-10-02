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

static cxBool cxAssetsStreamOpen(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxAssetsStream);
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(this->cxStream.path);
    this->asset = AAssetManager_open(cxEngineGetAssetManager(), path, AASSET_MODE_UNKNOWN);
    if(this->asset == NULL){
        CX_ERROR("open asset file %s failed",path);
        return false;
    }
    this->cxStream.Length = (cxInt)AAsset_getLength(this->asset);
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
    return AAsset_read(this->asset, buffer, size);
}

static cxInt cxAssetsStreamWrite(cxAny ps,cxAny buffer,cxInt size)
{
    CX_ASSERT_THIS(ps, cxAssetsStream);
    if(!this->cxStream.canWrite){
        CX_WARN("stream not support write");
        return 0;
    }
    return 0;
}

static cxOff cxAssetsStreamPosition(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxAssetsStream);
    if(!this->cxStream.canRead){
        CX_WARN("stream not support read");
        return 0;
    }
    return (cxOff)AAsset_seek(this->asset, 0, SEEK_CUR);
}

static cxInt cxAssetsStreamSeek(cxAny ps,cxOff off,cxInt flags)
{
    CX_ASSERT_THIS(ps, cxAssetsStream);
    if(!this->cxStream.canSeek){
        return false;
    }
    return AAsset_seek(this->asset, off, flags) > 0;
}

static cxString cxAssetsStreamAllBytes(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxStream);
    if(!this->canRead){
        cxStreamOpen(this);
    }
    if(!this->canRead){
        CX_ERROR("file stream can't read");
        return NULL;
    }
    cxStreamSeek(this,0,SEEK_SET);
    cxChar *bytes = allocator->malloc(this->Length + 1);
    cxStreamRead(this,bytes,this->Length);
    bytes[this->Length] = '\0';
    cxString data = cxStringAttachMem(bytes, this->Length);
    cxStreamClose(this);
    return data;
}

static void cxAssetsStreamClose(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxAssetsStream);
    if(this->asset != NULL){
        AAsset_close(this->asset);
        this->asset = NULL;
    }
    cxStreamBaseClose(this);
}

CX_OBJECT_TYPE(cxAssetsStream, cxStream)
{
}
CX_OBJECT_INIT(cxAssetsStream, cxStream)
{
    CX_SET(cxStream, this, Read, cxAssetsStreamRead);
    CX_SET(cxStream, this, Open, cxAssetsStreamOpen);
    CX_SET(cxStream, this, Write, cxAssetsStreamWrite);
    CX_SET(cxStream, this, Seek, cxAssetsStreamSeek);
    CX_SET(cxStream, this, Close, cxAssetsStreamClose);
    CX_SET(cxStream, this, Position, cxAssetsStreamPosition);
    CX_SET(cxStream, this, AllBytes, cxAssetsStreamAllBytes);
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
    return (cxStream)rv;
}







