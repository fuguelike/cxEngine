//
//  NormalMap.c
//  cxCore
//
//  Created by xuhua on 10/2/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "NormalMap.h"

static cxBool NormalMapTouch(cxAny pview,cxTouchItems *points)
{
    return false;
}

CX_OBJECT_TYPE(NormalMap, Map)
{
    
}
CX_OBJECT_INIT(NormalMap, Map)
{
    CX_SET(cxView, this, Touch, NormalMapTouch);
}
CX_OBJECT_FREE(NormalMap, Map)
{
    
}
CX_OBJECT_TERM(NormalMap, Map)