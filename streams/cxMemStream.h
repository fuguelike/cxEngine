//
//  cxMemStream.h
//  cxEngine
//  内存流实现
//  Created by xuhua on 9/30/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxMemStream_h
#define cxEngine_cxMemStream_h

#include "cxBase.h"
#include "cxStream.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxMemStream, cxStream)
    cxOff position;
    char *data;
    cxUInt allocSize;
CX_OBJECT_END(cxMemStream)

cxStream cxMemStreamCreateWithText(cxString txt);

CX_C_END

#endif
