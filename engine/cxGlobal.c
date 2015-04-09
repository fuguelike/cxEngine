//
//  cxGlobal.c
//  cxEngine
//
//  Created by xuhua on 3/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxGlobal.h"

void cxGlobalInit()
{
    cxCoreInit();
    cxJsonInit();
}

void cxGlobalClear()
{
    cxJsonClear();
    cxCoreClear();
}

void cxGlobalFree()
{
    cxJsonFree();
    cxCoreFree();
}