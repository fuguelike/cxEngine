//
//  cxMemStream.c
//  cxEngine
//
//  Created by xuhua on 9/30/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxMemStream.h"

static cxBool cxMemStreamOpen(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxMemStream);
    CX_ASSERT(this->cxStream.isOpen == false,"stream repeat open");
    this->allocSize = 128;
    this->data = allocator->malloc(this->allocSize);
    this->cxStream.canRead = true;
    this->cxStream.canWrite = true;
    this->cxStream.canSeek = true;
    this->cxStream.isOpen = true;
    return true;
}

static cxInt cxMemStreamRead(cxAny ps,cxAny buffer,cxInt size)
{
    CX_ASSERT_THIS(ps, cxMemStream);
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

static cxInt cxMemStreamWrite(cxAny ps,cxAny buffer,cxInt size)
{
    CX_ASSERT_THIS(ps, cxMemStream);
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

static cxOff cxMemStreamPosition(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxMemStream);
    return this->position;
}

static cxInt cxMemStreamSeek(cxAny ps,cxOff off,cxInt flags)
{
    CX_ASSERT_THIS(ps, cxMemStream);
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

static cxString cxMemStreamAllBytes(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxMemStream);
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, this->data, this->cxStream.Length);
    return rv;
}

static void cxMemStreamClose(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxMemStream);
    this->allocSize = 0;
    allocator->free(this->data);
    this->data = NULL;
    this->position = 0;
    cxStreamBaseClose(this);
}

CX_TYPE(cxMemStream, cxStream)
{
    
}
CX_INIT(cxMemStream, cxStream)
{
    CX_SET(cxStream, this, Read, cxMemStreamRead);
    CX_SET(cxStream, this, Open, cxMemStreamOpen);
    CX_SET(cxStream, this, Write, cxMemStreamWrite);
    CX_SET(cxStream, this, Seek, cxMemStreamSeek);
    CX_SET(cxStream, this, Close, cxMemStreamClose);
    CX_SET(cxStream, this, Position, cxMemStreamPosition);
    CX_SET(cxStream, this, AllBytes, cxMemStreamAllBytes);
}
CX_FREE(cxMemStream, cxStream)
{
    
}
CX_TERM(cxMemStream, cxStream)

cxStream cxMemStreamCreateWithText(cxString txt)
{
    cxStream stream = CX_CREATE(cxMemStream);
    if(!cxStreamOpen(stream)){
        return NULL;
    }
    if(txt != NULL){
        cxStreamWrite(stream,(cxAny)cxStringBody(txt),cxStringLength(txt));
    }
    return stream;
}










