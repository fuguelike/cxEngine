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
#include "cxUtil.h"

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
}
CX_OBJECT_TERM(cxStream, cxObject)

cxBool cxStreamOpen(cxAny pstream)
{
    CX_ASSERT_THIS(pstream, cxStream);
    return CX_METHOD_GET(false, this->Open, this);
}

cxInt cxStreamRead(cxAny pstream,cxAny buffer,cxInt size)
{
    CX_ASSERT_THIS(pstream, cxStream);
    return CX_METHOD_GET(0, this->Read,this,buffer,size);
}

cxInt cxStreamWrite(cxAny pstream,cxAny buffer,cxInt size)
{
    CX_ASSERT_THIS(pstream, cxStream);
    return CX_METHOD_GET(0, this->Write,this,buffer,size);
}

cxInt cxStreamSeek(cxAny pstream,cxOff off,cxInt flags)
{
    CX_ASSERT_THIS(pstream, cxStream);
    return CX_METHOD_GET(0, this->Seek,this,off,flags);
}

cxOff cxStreamPosition(cxAny pstream)
{
    CX_ASSERT_THIS(pstream, cxStream);
    return CX_METHOD_GET(0, this->Position, this);
}

cxInt cxStreamLength(cxAny pstream)
{
    CX_ASSERT_THIS(pstream, cxStream);
    return this->length;
}

void cxStreamClose(cxAny pstream)
{
    CX_ASSERT_THIS(pstream, cxStream);
    CX_METHOD_RUN(this->Close,this);
}

cxString cxStreamAllBytes(cxAny pstream)
{
    CX_ASSERT_THIS(pstream, cxStream);
    return CX_METHOD_GET(NULL, this->AllBytes,this);
}

void cxStreamBaseClose(cxAny pstream)
{
    CX_ASSERT_THIS(pstream, cxStream);
    this->length = 0;
    this->canRead = false;
    this->canWrite = false;
    this->canSeek = false;
    this->isOpen = false;
}












