//
//  cxMove.h
//  cxEngine
//
//  Created by xuhua on 10/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxMove_h
#define cxEngine_cxMove_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_DEF(cxMove, cxAction)
    cxVec2f startPos;
    cxVec2f position;
    cxVec2f posDelta;
    CX_FIELD_DEF(cxFloat Angle);
CX_END(cxMove, cxAction)

CX_FIELD_GET(cxMove, cxFloat, Angle);

void cxMoveSetPos(cxAny pav,cxVec2f pos);

cxMove cxMoveCreate(cxFloat dutation, cxVec2f pos);

CX_C_END

#endif
