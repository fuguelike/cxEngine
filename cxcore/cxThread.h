//
//  cxThread.h
//  cxCore
//
//  Created by xuhua on 10/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxThread_h
#define cxCore_cxThread_h

#include <pthread.h>
#include "cxCore.h"
#include "cxMemPool.h"

CX_C_BEGIN

cxBool cxThreadRun(cxAny (*run)(cxAny));

CX_C_END

#endif
