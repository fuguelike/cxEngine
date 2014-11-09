//
//  cxMMapStream.c
//  cxCore
//
//  Created by xuhua on 9/13/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <android/asset_manager.h>
#include <unistd.h>
#include <sys/stat.h>
#include <engine/cxUtil.h>
#include <streams/cxMMapStream.h>
#include "cxAndroid.h"

//int AAsset_openFileDescriptor(AAsset* asset, off_t* outStart, off_t* outLength);
//int AAsset_isAllocated(AAsset* asset);
//const void* AAsset_getBuffer(AAsset* asset);
CX_METHOD_DEF(cxMMapStream,Open,cxBool)
{
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(this->cxStream.path);
    this->asset = AAssetManager_open(cxEngineGetAssetManager(), path, AASSET_MODE_UNKNOWN);
    if(this->asset == NULL){
        CX_ERROR("open asset file %s failed",path);
        return false;
    }
    this->map = (cxAny)AAsset_getBuffer(this->asset);
    if(this->map == NULL){
        CX_ERROR("asset get buffer error");
        return false;
    }
    this->off = 0;
    this->cxStream.Length = (cxInt)AAsset_getLength(this->asset);
    this->cxStream.canRead = true;
    this->cxStream.canSeek = true;
    this->cxStream.canWrite = false;
    this->cxStream.isOpen = true;
    return true;
}
CX_METHOD_DEF(cxMMapStream,Read,cxInt,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canRead){
        return 0;
    }
    cxInt bytes = (cxInt)(this->cxStream.Length - this->position);
    if(bytes <= 0){
        return 0;
    }
    if(bytes > size){
        bytes = size;
    }
    memcpy(buffer, this->map + this->position, bytes);
    this->position += bytes;
    return bytes;
}
CX_METHOD_DEF(cxMMapStream,Write,cxInt,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canWrite){
        return 0;
    }
    cxInt bytes = (cxInt)(this->cxStream.Length - this->position);
    if(size > bytes){
        size = bytes;
    }
    memcpy(this->map + this->position, buffer, size);
    this->position += size;
    this->cxStream.Length += size;
    return size;
}
CX_METHOD_DEF(cxMMapStream,Position,cxInt)
{
    return this->position;
}
CX_METHOD_DEF(cxMMapStream,Seek,cxInt,cxInt off,cxInt flags)
{
    if(!this->cxStream.canSeek){
        return 0;
    }
    if(flags == SEEK_SET && off < this->cxStream.Length){
        this->position = off;
        return this->position;
    }
    cxInt seek = (cxInt)(this->cxStream.Length - this->position);
    if(flags == SEEK_CUR && off < seek){
        this->position += off;
        return this->position;
    }
    seek = (cxInt)(this->cxStream.Length + off);
    if(flags == SEEK_END && seek < this->cxStream.Length){
        this->position = seek;
        return this->position;
    }
    return 0;
}
CX_METHOD_DEF(cxMMapStream,AllBytes,cxString)
{
    if(!this->cxStream.canRead){
        CX_CALL(this, Open, CX_M(void));
    }
    if(!this->cxStream.canRead){
        CX_ERROR("file %s stream can't open",cxStringBody(this->cxStream.path));
        return NULL;
    }
    CX_CALL(this, Seek, CX_M(cxInt,cxInt,cxInt),0,SEEK_SET);
    return cxStringNoCopy(this->map, this->cxStream.Length);
}
CX_METHOD_DEF(cxMMapStream,Close,void)
{
    if(this->map != NULL){
        this->map = NULL;
    }
    if(this->asset != NULL){
        AAsset_close(this->asset);
        this->asset = NULL;
    }
    CX_SUPER(cxStream, this, Close, CX_M(void));
}
CX_TYPE(cxMMapStream, cxStream)
{
    CX_METHOD(cxMMapStream, Open);
    CX_METHOD(cxMMapStream, Read);
    CX_METHOD(cxMMapStream, Write);
    CX_METHOD(cxMMapStream, Seek);
    CX_METHOD(cxMMapStream, Position);
    CX_METHOD(cxMMapStream, Close);
    CX_METHOD(cxMMapStream, AllBytes);
}
CX_INIT(cxMMapStream, cxStream)
{
    this->fd = -1;
    this->flags = CX_MMAP_READ;
}
CX_FREE(cxMMapStream, cxStream)
{

}
CX_TERM(cxMMapStream, cxStream)

cxStream cxMMapStreamCreate(cxConstChars file)
{
    cxMMapStream this = CX_CREATE(cxMMapStream);
    CX_RETAIN_SWAP(this->cxStream.path, cxStringConstChars(file));
    return (cxStream)this;
}









