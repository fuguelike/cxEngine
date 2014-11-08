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
#include "cxString.h"

CX_C_BEGIN

CX_DEF(cxStream, cxObject)
    cxString path;
    CX_FIELD_DEF(cxInt Length);
    cxBool canRead;
    cxBool canWrite;
    cxBool canSeek;
    cxBool isOpen;
CX_END(cxStream, cxObject)

CX_FIELD_IMP(cxStream, cxInt, Length);

CX_C_END

#endif










