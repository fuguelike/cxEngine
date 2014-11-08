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

static cxBool CX_METHOD(cxAssetsStream,Open)
{
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

static cxInt CX_METHOD(cxAssetsStream,Read,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canRead){
        return 0;
    }
    return AAsset_read(this->asset, buffer, size);
}

static cxInt CX_METHOD(cxAssetsStream,Write,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canWrite){
        CX_WARN("stream not support write");
        return 0;
    }
    return 0;
}

static cxOff CX_METHOD(cxAssetsStream,Position)
{
    if(!this->cxStream.canRead){
        CX_WARN("stream not support read");
        return 0;
    }
    return (cxOff)AAsset_seek(this->asset, 0, SEEK_CUR);
}

static cxInt CX_METHOD(cxAssetsStream,Seek,cxOff off,cxInt flags)
{
    if(!this->cxStream.canSeek){
        return false;
    }
    return AAsset_seek(this->asset, off, flags) > 0;
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

static void CX_METHOD(cxAssetsStream,Close)
{
    if(this->asset != NULL){
        AAsset_close(this->asset);
        this->asset = NULL;
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








