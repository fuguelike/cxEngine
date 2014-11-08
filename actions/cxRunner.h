//
//  cxRunner.h
//  cxEngine
//
//  Created by xuhua on 11/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxRunner_h
#define cxEngine_cxRunner_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_DEF(cxRunner, cxAction)
    cxAny data;
    cxInt count;
    cxInt step;
CX_END(cxRunner, cxAction)

void cxRunnerAppend(cxAny runner,cxAny pav, cxAny pview);

cxRunner cxRunnerCreate(cxAny data);

CX_C_END

#endif
