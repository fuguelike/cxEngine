//
//  cxTint.h
//  cxEngine
//  色彩
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTint_h
#define cxEngine_cxTint_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxTint, cxAction)
    cxColor4f delta;
    cxColor4f start;
    cxColor4f color;
CX_OBJECT_END(cxTint, cxAction)

cxTint cxTintCreate(cxFloat time,cxColor4f color);

CX_C_END

#endif
