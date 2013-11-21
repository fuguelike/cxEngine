//
//  cxRunner.h
//  cxEngine
//
//  Created by xuhua on 11/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngineIOS_cxRunner_h
#define cxEngineIOS_cxRunner_h

#include <core/cxAction.h>

CX_C_BEGIN

typedef void (*cxRunnerFunc)(cxAny pav);

CX_OBJECT_DEF(cxRunner, cxAction)
    cxInt count;
    cxInt step;
    CX_METHOD_DEF(cxRunnerFunc, Init);
    CX_METHOD_DEF(cxRunnerFunc, Exit);
CX_OBJECT_END(cxRunner)

void cxRunnerAppend(cxAny runner,cxAny pav, cxAny pview);

CX_C_END

#endif
