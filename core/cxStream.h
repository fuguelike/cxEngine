//
//  cxStream.h
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxStream_h
#define cxEngine_cxStream_h

#include "cxBase.h"
#include "cxString.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxStream, cxObject)
    cxString path;
    cxString file;
    cxInt length;
    cxBool canRead;
    cxBool canWrite;
    cxBool canSeek;
    cxBool isOpen;
    CX_METHOD_ALLOC(Open);
    CX_METHOD_ALLOC(Read);
    CX_METHOD_ALLOC(Write);
    CX_METHOD_ALLOC(Seek);
    CX_METHOD_ALLOC(Position);
    CX_METHOD_ALLOC(Close);
    CX_METHOD_ALLOC(AllBytes);
CX_OBJECT_END(cxStream)

void cxStreamBaseClose(cxAny this);

cxString cxStreamFileDir(cxAny this);

cxBool cxStreamOpen(cxAny this);

cxInt cxStreamRead(cxAny this,cxPointer buffer,cxInt size);

cxInt cxStreamWrite(cxAny this,cxPointer buffer,cxInt size);

cxBool cxStreamSeek(cxAny this,cxOff off,cxInt flags);

cxOff cxStreamPosition(cxAny this);

cxInt cxStreamLength(cxAny this);

void cxStreamClose(cxAny this);

cxString cxStreamAllBytes(cxAny this);

CX_C_END

#endif










