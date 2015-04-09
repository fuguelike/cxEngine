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
CX_METHOD_DEF(cxMMapStream,Open,cxBool)
{
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    cxConstChars file = cxStrBody(this->cxStream.path);
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
CX_METHOD_DEF(cxMMapStream,AllBytes,cxStr)
{
    if(!this->cxStream.canRead){
        CX_CALL(this, Open, CX_M(void));
    }
    if(!this->cxStream.canRead){
        CX_ERROR("file %s stream can't open",cxStrBody(this->cxStream.path));
        return NULL;
    }
    CX_CALL(this, Seek, CX_M(cxInt,cxInt,cxInt),0,SEEK_SET);
    //return memory ref
    return cxStrNoCopy(this->map, this->cxStream.Length);
}
CX_METHOD_DEF(cxMMapStream,Close,void)
{
    if(this->fd >= 0){
        close(this->fd);
        this->fd = -1;
    }
    if(this->map != MAP_FAILED){
        munmap(this->map, this->cxStream.Length);
        this->map = MAP_FAILED;
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
    CX_RETAIN_SWAP(this->cxStream.path, cxStrConstChars(file));
    return (cxStream)this;
}









