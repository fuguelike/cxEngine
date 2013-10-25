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
    cxVec2f endPos;
    cxVec2f prevPos;
    cxVec2f currPos;
    cxVec2f begPos;
    cxVec2f posDelta;
CX_OBJECT_END(cxMove)

cxMove cxMoveCreate(cxFloat dutation, cxVec2f endpos);

CX_C_END

#endif
