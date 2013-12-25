//
//  cxRunner.h
//  cxEngine
//
//  Created by xuhua on 11/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxRunner_h
#define cxEngine_cxRunner_h

#include <core/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxRunner, cxAction)
    cxInt count;
    cxInt step;
    CX_METHOD_ALLOC(Init);
    CX_METHOD_ALLOC(Exit);
CX_OBJECT_END(cxRunner)

void cxRunnerAppend(cxAny runner,cxAny pav, cxAny pview);

CX_C_END

#endif
