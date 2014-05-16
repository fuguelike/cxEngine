//
//  cxUrlPath.h
//  cxEngine
//
//  Created by xuhua on 12/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxUrlPath_h
#define cxEngine_cxUrlPath_h

#include "cxBase.h"
#include "cxHash.h"

CX_C_BEGIN

//path?key=value
CX_OBJECT_DEF(cxUrlPath, cxObject)
    cxChar path[CX_HASH_MAX_KEY_LENGTH];
    cxChar key[CX_HASH_MAX_KEY_LENGTH];
    cxChar value[CX_HASH_MAX_KEY_LENGTH];
    cxInt count;
CX_OBJECT_END(cxUrlPath)

cxUrlPath cxUrlPathParse(cxConstChars url);

CX_C_END

#endif
