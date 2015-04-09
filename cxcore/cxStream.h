//
//  cxStream.h
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxStream_h
#define cxEngine_cxStream_h

#include "cxCore.h"
#include "cxStr.h"

CX_C_BEGIN

CX_DEF(cxStream, cxObject)
    cxStr path;
    CX_FIELD_DEF(cxInt Length);
    cxBool canRead;
    cxBool canWrite;
    cxBool canSeek;
    cxBool isOpen;
CX_END(cxStream, cxObject)

CX_FIELD_IMP(cxStream, cxInt, Length);

#define cxStreamOpen(stream)                CX_CALL(stream, Open, CX_M(cxBool))

#define cxStreamRead(stream,buff,bytes)     CX_CALL(stream, Read, CX_M(cxInt,cxAny,cxInt), buff, bytes)

#define cxStreamWrite(stream,buff,bytes)    CX_CALL(stream, Write, CX_M(cxInt,cxAny,cxInt), buff, bytes)

#define cxStreamSeek(stream,seek,where)     CX_CALL(stream, Seek, CX_M(cxInt,cxInt,cxInt), seek, where)

#define cxStreamPosition(stream)            CX_CALL(stream, Position, CX_M(cxInt))

#define cxStreamClose(stream)               CX_CALL(stream, Close, CX_M(void))

#define cxStreamAllBytes(stream)            CX_CALL(stream, AllBytes, CX_M(cxStr))

CX_C_END

#endif










