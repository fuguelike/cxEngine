//
//  cxFileStream.h
//  cxEngine
//
//  Created by xuhua on 12/16/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFileStream_h
#define cxEngine_cxFileStream_h

#include <cxcore/cxStream.h>

CX_C_BEGIN

//document file stream

CX_DEF(cxFileStream, cxStream)
    FILE *fd;
    cxBool rdonly;
CX_END(cxFileStream, cxStream)

cxString cxDocumentData(cxConstChars file);

cxStream cxFileStreamCreate(cxConstChars file,cxBool rdonly);

CX_C_END

#endif
