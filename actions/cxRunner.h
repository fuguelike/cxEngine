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
    cxAny core;
    cxInt count;
    cxInt step;
    CX_METHOD_ALLOC(void, Init, cxAny);
    CX_METHOD_ALLOC(void, Exit, cxAny);
CX_OBJECT_END(cxRunner)

void cxRunnerAppend(cxAny runner,cxAny pav, cxAny pview);

cxRunner cxRunnerCreate(cxAny core);

CX_C_END

#endif
