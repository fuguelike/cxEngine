//
//  cxStrStream.c
//  cxEngineStatic
//
//  Created by xuhua on 11/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxStrStream.h"

CX_METHOD_DEF(cxStrStream,Open,cxBool)
{
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    this->cxStream.Length = cxStrLength(this->data);
    this->cxStream.isOpen = true;
    this->cxStream.canRead = true;
    this->cxStream.canWrite = false;
    this->cxStream.canSeek = true;
    return true;
}
CX_METHOD_DEF(cxStrStream,Read,cxInt,cxAny buffer,cxInt size)
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
    cxAny data = cxStrBody(this->data);
    memcpy(buffer, data + this->position, bytes);
    this->position += bytes;
    return bytes;
}
CX_METHOD_DEF(cxStrStream,Write,cxInt,cxAny buffer,cxInt size)
{
    return 0;
}
CX_METHOD_DEF(cxStrStream,Position,cxInt)
{
    return this->position;
}
CX_METHOD_DEF(cxStrStream,Seek,cxInt,cxInt off,cxInt flags)
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
CX_METHOD_DEF(cxStrStream,AllBytes,cxStr)
{
    return this->data;
}
CX_METHOD_DEF(cxStrStream,Close,void)
{
    CX_SUPER(cxStream, this, Close, CX_M(void));
}
CX_TYPE(cxStrStream, cxStream)
{
    CX_METHOD(cxStrStream, Open);
    CX_METHOD(cxStrStream, Read);
    CX_METHOD(cxStrStream, Write);
    CX_METHOD(cxStrStream, Seek);
    CX_METHOD(cxStrStream, Position);
    CX_METHOD(cxStrStream, Close);
    CX_METHOD(cxStrStream, AllBytes);
}
CX_INIT(cxStrStream, cxStream)
{
    this->data = CX_ALLOC(cxStr);
}
CX_FREE(cxStrStream, cxStream)
{
    CX_RELEASE(this->data);
}
CX_TERM(cxStrStream, cxStream)

cxStream cxStrStreamCreate(cxStr data)
{
    CX_ASSERT(cxStrOK(data), "data args error");
    cxStrStream this = CX_CREATE(cxStrStream);
    CX_RETAIN_SWAP(this->data, data);
    return (cxStream)this;
}









