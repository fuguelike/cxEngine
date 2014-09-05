//
//  cxMemStream.c
//  cxEngine
//
//  Created by xuhua on 9/30/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxMemStream.h"

static cxBool cxMemStreamOpen(cxAny this)
{
    cxMemStream stream = this;
    CX_ASSERT(stream->cxStream.isOpen == false,"stream repeat open");
    stream->allocSize = 128;
    stream->data = allocator->malloc(stream->allocSize);
    stream->cxStream.canRead = true;
    stream->cxStream.canWrite = true;
    stream->cxStream.canSeek = true;
    stream->cxStream.isOpen = true;
    return true;
}

static cxInt cxMemStreamRead(cxAny this,cxAny buffer,cxInt size)
{
    cxMemStream stream = this;
    if(!stream->cxStream.canRead){
        return 0;
    }
    cxInt bytes = (cxInt)(stream->cxStream.length - stream->position);
    if(bytes <= 0){
        return 0;
    }
    if(bytes > size){
        bytes = size;
    }
    memcpy(buffer, stream->data + stream->position, bytes);
    stream->position += bytes;
    return bytes;
}

static cxInt cxMemStreamWrite(cxAny this,cxAny buffer,cxInt size)
{
    cxMemStream stream = this;
    if(!stream->cxStream.canWrite){
        return 0;
    }
    cxInt bytes = (cxInt)(stream->allocSize - stream->position);
    if(bytes < size){
        stream->allocSize += (size + 128);
        stream->data = allocator->realloc(stream->data,stream->allocSize);
        CX_ASSERT(stream->data != NULL, "memory realloc failed");
    }
    memcpy(stream->data + stream->position, buffer, size);
    stream->position += size;
    stream->cxStream.length += size;
    return size;
}

static cxOff cxMemStreamPosition(cxAny this)
{
    cxMemStream stream = this;
    return stream->position;
}

static cxInt cxMemStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxMemStream stream = this;
    if(!stream->cxStream.canSeek){
        return 0;
    }
    if(flags == SEEK_SET && off < stream->cxStream.length){
        stream->position = off;
        return stream->position;
    }
    cxInt seek = (cxInt)(stream->cxStream.length - stream->position);
    if(flags == SEEK_CUR && off < seek){
        stream->position += off;
        return stream->position;
    }
    seek = (cxInt)(stream->cxStream.length + off);
    if(flags == SEEK_END && seek < stream->cxStream.length){
        stream->position = seek;
        return stream->position;
    }
    return 0;
}

static cxString cxMemStreamAllBytes(cxAny this)
{
    cxMemStream stream = this;
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, stream->data, stream->cxStream.length);
    return rv;
}

static void cxMemStreamClose(cxAny this)
{
    cxMemStream stream = this;
    stream->allocSize = 0;
    allocator->free(stream->data);
    stream->data = NULL;
    stream->position = 0;
    cxStreamBaseClose(this);
}

CX_OBJECT_TYPE(cxMemStream, cxStream)
{
    
}
CX_OBJECT_INIT(cxMemStream, cxStream)
{
    CX_METHOD_SET(this->cxStream.Open, cxMemStreamOpen);
    CX_METHOD_SET(this->cxStream.Read, cxMemStreamRead);
    CX_METHOD_SET(this->cxStream.Write, cxMemStreamWrite);
    CX_METHOD_SET(this->cxStream.Seek, cxMemStreamSeek);
    CX_METHOD_SET(this->cxStream.Close, cxMemStreamClose);
    CX_METHOD_SET(this->cxStream.Position,cxMemStreamPosition);
    CX_METHOD_SET(this->cxStream.AllBytes,cxMemStreamAllBytes);
}
CX_OBJECT_FREE(cxMemStream, cxStream)
{
    
}
CX_OBJECT_TERM(cxMemStream, cxStream)

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










