//
//  cxEventItem.h
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxEventItem_h
#define cxEngine_cxEventItem_h

#include "cxBase.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxEventItem, cxObject)
    cxEventFunc func;
    cxAny arg;
CX_OBJECT_END(cxEventItem)

CX_C_END

#endif
