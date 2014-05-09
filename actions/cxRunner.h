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
    cxAny data;
    cxInt count;
    cxInt step;
    CX_METHOD_DEF(void, Init, cxAny);
    CX_METHOD_DEF(void, Exit, cxAny);
CX_OBJECT_END(cxRunner)

void cxRunnerAppend(cxAny runner,cxAny pav, cxAny pview);

cxRunner cxRunnerCreate(cxAny data);

CX_C_END

#endif
