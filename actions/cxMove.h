//
//  cxMove.h
//  cxEngine
//
//  Created by xuhua on 10/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxMove_h
#define cxEngine_cxMove_h

#include <core/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxMove, cxAction)
    cxVec2f oldPos;
    cxVec2f delta;
    cxVec2f newPos;
CX_OBJECT_END(cxMove)

cxMove cxMoveCreate(cxFloat dutation, cxVec2f newpos);

CX_C_END

#endif
