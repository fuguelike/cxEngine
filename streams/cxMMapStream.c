//
//  cxMMapStream.c
//  cxCore
//
//  Created by xuhua on 9/13/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <engine/cxUtil.h>
#include "cxMMapStream.h"

//MAP_FAILED
static cxBool CX_METHOD(cxMMapStream,Open)
{
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    cxConstChars file = cxStringBody(this->cxStream.path);
    cxInt length  = 0;
    this->fd = cxAssertsFD(file, &this->off, &length);
    if(this->fd < 0){
        CX_ERROR("get assert %s fd=%d failed %d/%d",file,this->fd,this->off,length);
        return false;
    }
    cxStreamSetLength(this, length);
    this->cxStream.canSeek = true;
    cxInt f = 0;
    if(this->flags & CX_MMAP_READ){
        f |= PROT_READ;
    }
    if(this->flags & CX_MMAP_WRITE){
        f |= PROT_WRITE;
    }
    this->map = mmap(NULL, this->cxStream.Length, f, MAP_SHARED, this->fd, this->off);
    if(this->map == MAP_FAILED){
        return false;
    }
    if(this->flags & CX_MMAP_READ){
        this->cxStream.canRead = true;
    }
    if(this->flags & CX_MMAP_WRITE){
        this->cxStream.canWrite = true;
    }
    this->cxStream.isOpen = true;
    return true;
}

static cxInt CX_METHOD(cxMMapStream,Read,cxAny buffer,cxInt size)
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

static cxInt CX_METHOD(cxMMapStream,Write,cxAny buffer,cxInt size)
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

static cxOff CX_METHOD(cxMMapStream,Position)
{
    return this->position;
}

static cxInt CX_METHOD(cxMMapStream,Seek,cxOff off,cxInt flags)
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

static cxString CX_METHOD(cxMMapStream,AllBytes)
{
    if(!this->cxStream.canRead){
        CX_CALL(this, Open, CX_MT(void));
    }
    if(!this->cxStream.canRead){
        CX_ERROR("file %s stream can't open",cxStringBody(this->cxStream.path));
        return NULL;
    }
    CX_CALL(this, Seek, CX_MT(cxInt,cxOff,cxInt),0,SEEK_SET);
    //return memory ref
    return cxStringNoCopy(this->map, this->cxStream.Length);
}

static void CX_METHOD(cxMMapStream,Close)
{
    if(this->fd >= 0){
        close(this->fd);
        this->fd = -1;
    }
    if(this->map != MAP_FAILED){
        munmap(this->map, this->cxStream.Length);
        this->map = MAP_FAILED;
    }
    CX_SUPER(cxStream, this, Close, CX_MT(void));
}

CX_TYPE(cxMMapStream, cxStream)
{
    CX_MSET(cxMMapStream, Open);
    CX_MSET(cxMMapStream, Read);
    CX_MSET(cxMMapStream, Write);
    CX_MSET(cxMMapStream, Seek);
    CX_MSET(cxMMapStream, Position);
    CX_MSET(cxMMapStream, Close);
    CX_MSET(cxMMapStream, AllBytes);
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









