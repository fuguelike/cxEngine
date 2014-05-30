//
//  cxUrlPath.h
//  cxEngine
//
//  Created by xuhua on 12/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxUrlPath_h
#define cxEngine_cxUrlPath_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

//path?key=value
CX_OBJECT_DEF(cxUrlPath, cxObject)
    cxChar path[CX_MAX_KEY];
    cxChar key[CX_MAX_KEY];
    cxChar value[CX_MAX_KEY];
    cxInt count;
CX_OBJECT_END(cxUrlPath, cxObject)

cxUrlPath cxUrlPathParse(cxConstChars url);

CX_C_END

#endif
