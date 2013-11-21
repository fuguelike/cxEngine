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

typedef cxBool (*cxStreamOpenFunc)(cxAny this);

typedef cxInt (*cxStreamReadFunc)(cxAny this,cxPointer buffer,cxInt size);

typedef cxInt (*cxStreamWriteFunc)(cxAny this,cxPointer buffer,cxInt size);

typedef cxBool (*cxStreamSeekFunc)(cxAny this,cxOff off,cxInt flags);

typedef cxOff (*cxStreamPositionFunc)(cxAny this);

typedef void (*cxStreamCloseFunc)(cxAny this);

typedef cxString (*cxStreamAllBytesFunc)(cxAny this);

CX_OBJECT_DEF(cxStream, cxObject)
    cxString path;
    cxString file;
    cxSize length;
    cxBool canRead;
    cxBool canWrite;
    cxBool canSeek;
    cxBool isOpen;
    CX_METHOD_DEF(cxStreamOpenFunc, Open);
    CX_METHOD_DEF(cxStreamReadFunc, Read);
    CX_METHOD_DEF(cxStreamWriteFunc, Write);
    CX_METHOD_DEF(cxStreamSeekFunc, Seek);
    CX_METHOD_DEF(cxStreamPositionFunc, Position);
    CX_METHOD_DEF(cxStreamCloseFunc, Close);
    CX_METHOD_DEF(cxStreamAllBytesFunc, AllBytes);
CX_OBJECT_END(cxStream)

void cxStreamBaseClose(cxAny this);

cxString cxStreamFileDir(cxAny this);

cxBool cxStreamOpen(cxAny this);

cxInt cxStreamRead(cxAny this,cxPointer buffer,cxInt size);

cxInt cxStreamWrite(cxAny this,cxPointer buffer,cxInt size);

cxBool cxStreamSeek(cxAny this,cxOff off,cxInt flags);

cxOff cxStreamPosition(cxAny this);

void cxStreamClose(cxAny this);

cxString cxStreamAllBytes(cxAny this);

CX_C_END

#endif










