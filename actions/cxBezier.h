//
//  cxBezier.h
//  cxEngine
//
//  Created by xuhua on 8/26/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxBezier_h
#define cxEngine_cxBezier_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxBezier, cxAction)
    cxVec2f epos;   //end point
    cxVec2f cpos1;  //controller point1
    cxVec2f cpos2;  //controller point2
    cxVec2f from;
    cxVec2f diff;
    cxVec2f prev;
CX_OBJECT_END(cxBezier, cxAction)

cxBezier cxBezierCreate(cxFloat time,cxVec2f epos,cxVec2f cpos1,cxVec2f cpos2);

CX_C_END

#endif
