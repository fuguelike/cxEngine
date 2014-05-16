//
//  cxFileStream.h
//  cxEngine
//
//  Created by xuhua on 12/16/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFileStream_h
#define cxEngine_cxFileStream_h

#include <engine/cxStream.h>

CX_C_BEGIN

//document file stream

CX_OBJECT_DEF(cxFileStream, cxStream)
    FILE *fd;
CX_OBJECT_END(cxFileStream)

cxStream cxFileStreamCreate(cxConstChars file);

CX_C_END

#endif
