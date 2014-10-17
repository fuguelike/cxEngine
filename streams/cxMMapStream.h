//
//  cxMMapStream.h
//  cxEngine
//  from assert file mmap
//  Created by xuhua on 9/13/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxMMapStream_h
#define cxEngine_cxMMapStream_h

#include <cxcore/cxStream.h>

CX_C_BEGIN

#define CX_MMAP_READ    (1 << 0)
#define CX_MMAP_WRITE   (1 << 1)

CX_DEF(cxMMapStream, cxStream)
    cxUInt flags;
    cxAny map;
    cxInt off;
    cxInt fd;
    cxInt position;
    cxAny asset;
CX_END(cxMMapStream, cxStream)

cxStream cxMMapStreamCreate(cxConstChars file);

CX_C_END

#endif
