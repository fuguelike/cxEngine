//
//  cxTint.h
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTint_h
#define cxEngine_cxTint_h

#include <core/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxTint, cxAction)
    cxColor3f delta;
    cxColor3f start;
    cxColor3f color;
CX_OBJECT_END(cxTint)

cxTint cxTintCreate(cxFloat duration,cxColor3f color);

CX_C_END

#endif
