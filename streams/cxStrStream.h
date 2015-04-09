//
//  cxStrStream.h
//  cxEngine
//
//  Created by xuhua on 11/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxStrStream_h
#define cxEngine_cxStrStream_h

#include <cxcore/cxStream.h>

CX_C_BEGIN

CX_DEF(cxStrStream, cxStream)
    cxInt position;
    cxStr data;
CX_END(cxStrStream, cxStream)

cxStream cxStrStreamCreate(cxStr data);

CX_C_END

#endif
