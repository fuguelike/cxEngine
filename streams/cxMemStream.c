//
//  cxMemStream.c
//  cxEngine
//
//  Created by xuhua on 9/30/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxMemStream.h"

CX_METHOD_DEF(cxMemStream,Open,cxBool)
{
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    this->allocSize = 128;
    this->data = allocator->malloc(this->allocSize);
    this->cxStream.canRead = true;
    this->cxStream.canWrite = true;
    this->cxStream.canSeek = true;
    this->cxStream.isOpen = true;
    return true;
}
CX_METHOD_DEF(cxMemStream,Read,cxInt,cxAny buffer,cxInt size)
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
    memcpy(buffer, this->data + this->position, bytes);
    this->position += bytes;
    return bytes;
}
CX_METHOD_DEF(cxMemStream,Write,cxInt,cxAny buffer,cxInt size)
{
    if(!this->cxStream.canWrite){
        return 0;
    }
    cxInt bytes = (cxInt)(this->allocSize - this->position);
    if(bytes < size){
        this->allocSize += (size + 128);
        this->data = allocator->realloc(this->data,this->allocSize);
        CX_ASSERT(this->data != NULL, "memory realloc failed");
    }
    memcpy(this->data + this->position, buffer, size);
    this->position += size;
    this->cxStream.Length += size;
    return size;
}
CX_METHOD_DEF(cxMemStream,Position,cxInt)
{
    return this->position;
}
CX_METHOD_DEF(cxMemStream,Seek,cxInt,cxInt off,cxInt flags)
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
CX_METHOD_DEF(cxMemStream,AllBytes,cxString)
{
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, this->data, this->cxStream.Length);
    return rv;
}
CX_METHOD_DEF(cxMemStream,Close,void)
{
    this->allocSize = 0;
    if(this->data != NULL){
        allocator->free(this->data);
        this->data = NULL;
    }
    this->position = 0;
    CX_SUPER(cxStream, this, Close, CX_M(void));
}
CX_TYPE(cxMemStream, cxStream)
{
    CX_METHOD(cxMemStream, Open);
    CX_METHOD(cxMemStream, Read);
    CX_METHOD(cxMemStream, Write);
    CX_METHOD(cxMemStream, Seek);
    CX_METHOD(cxMemStream, Position);
    CX_METHOD(cxMemStream, Close);
    CX_METHOD(cxMemStream, AllBytes);
}
CX_INIT(cxMemStream, cxStream)
{
    
}
CX_FREE(cxMemStream, cxStream)
{
    
}
CX_TERM(cxMemStream, cxStream)

cxStream cxMemStreamCreateWithText(cxString txt)
{
    cxStream stream = CX_CREATE(cxMemStream);
    if(!CX_CALL(stream, Open, CX_M(cxBool))){
        return NULL;
    }
    if(txt != NULL){
        CX_CALL(stream, Write, CX_M(cxInt,cxAny,cxInt),cxStringBody(txt),cxStringLength(txt));
    }
    return stream;
}










