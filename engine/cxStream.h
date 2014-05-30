//
//  cxStream.h
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxStream_h
#define cxEngine_cxStream_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxStream, cxObject)
    cxString path;
    cxString file;
    cxInt length;
    cxBool canRead;
    cxBool canWrite;
    cxBool canSeek;
    cxBool isOpen;
    CX_METHOD_DEF(cxBool, Open, cxAny);
    CX_METHOD_DEF(cxInt, Read, cxAny, cxPointer, cxInt);
    CX_METHOD_DEF(cxInt, Write, cxAny, cxPointer, cxInt);
    CX_METHOD_DEF(cxInt, Seek, cxAny, cxOff, cxInt);
    CX_METHOD_DEF(cxOff, Position, cxAny);
    CX_METHOD_DEF(void, Close, cxAny);
    CX_METHOD_DEF(cxString, AllBytes, cxAny);
CX_OBJECT_END(cxStream, cxObject)

void cxStreamBaseClose(cxAny this);

cxString cxStreamFileDir(cxAny this);

cxBool cxStreamOpen(cxAny this);

cxInt cxStreamRead(cxAny this,cxPointer buffer,cxInt size);

cxInt cxStreamWrite(cxAny this,cxPointer buffer,cxInt size);

cxInt cxStreamSeek(cxAny this,cxOff off,cxInt flags);

cxOff cxStreamPosition(cxAny this);

cxInt cxStreamLength(cxAny this);

void cxStreamClose(cxAny this);

cxString cxStreamAllBytes(cxAny this);

CX_C_END

#endif










