//
//  cxStream.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <libgen.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxMemStream.h>
#include <streams/cxFileStream.h>
#include "cxStream.h"

void __cxStreamInitType(cxAny type)
{
    
}

void __cxStreamInitObject(cxAny object,cxAny json,cxAny hash)
{
    
}

CX_OBJECT_TYPE(cxStream, cxObject)
{
    
}
CX_OBJECT_INIT(cxStream, cxObject)
{
    //
}
CX_OBJECT_FREE(cxStream, cxObject)
{
    if(this->isOpen){
        cxStreamClose(this);
    }
    CX_RELEASE(this->path);
    CX_RELEASE(this->file);
}
CX_OBJECT_TERM(cxStream, cxObject)

cxBool cxStreamOpen(cxAny this)
{
    cxStream stream = this;
    return CX_METHOD_GET(false, stream->Open, this);
}

cxInt cxStreamRead(cxAny this,cxAny buffer,cxInt size)
{
    cxStream stream = this;
    return CX_METHOD_GET(0, stream->Read,this,buffer,size);
}

cxInt cxStreamWrite(cxAny this,cxAny buffer,cxInt size)
{
    cxStream stream = this;
    return CX_METHOD_GET(0, stream->Write,this,buffer,size);
}

cxInt cxStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxStream stream = this;
    return CX_METHOD_GET(0, stream->Seek,this,off,flags);
}

cxOff cxStreamPosition(cxAny this)
{
    cxStream stream = this;
    return CX_METHOD_GET(0, stream->Position, this);
}

cxInt cxStreamLength(cxAny this)
{
    cxStream stream = this;
    return stream->length;
}

void cxStreamClose(cxAny this)
{
    cxStream stream = this;
    CX_METHOD_RUN(stream->Close,this);
}

cxString cxStreamAllBytes(cxAny this)
{
    cxStream stream = this;
    return CX_METHOD_GET(NULL, stream->AllBytes,this);
}

void cxStreamBaseClose(cxAny this)
{
    cxStream stream = this;
    stream->length = 0;
    stream->canRead = false;
    stream->canWrite = false;
    stream->canSeek = false;
    stream->isOpen = false;
}

cxString cxStreamFileDir(cxAny this)
{
    cxStream stream = this;
    if(stream->path == NULL){
        return cxStringCreate(".");
    }
    char *dir = dirname((char *)cxStringBody(stream->file));
    return cxStringCreate("%s",dir);
}












