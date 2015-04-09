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

typedef enum {
    cxBezierType2,
    cxBezierType3,
}cxBezierType;

CX_DEF(cxBezier, cxAction)
    CX_FIELD_DEF(cxBezierType Type;)
    CX_FIELD_DEF(cxVec2f Epos);     //end point
    CX_FIELD_DEF(cxVec2f Pos1);     //controller point1
    CX_FIELD_DEF(cxVec2f Pos2);     //controller point2
    CX_FIELD_DEF(cxVec2f Offset);   //start offset
    CX_FIELD_DEF(cxFloat Angle);    //angle change
    cxVec2f from;
CX_END(cxBezier, cxAction)

CX_FIELD_GET(cxBezier, cxFloat, Angle);
CX_FIELD_SET(cxBezier, cxVec2f, Offset);
CX_FIELD_SET(cxBezier, cxBezierType, Type);
CX_FIELD_SET(cxBezier, cxVec2f, Epos);
CX_FIELD_SET(cxBezier, cxVec2f, Pos1);
CX_FIELD_SET(cxBezier, cxVec2f, Pos2);

cxBezier cxBezier3Create(cxFloat time,cxVec2f epos,cxVec2f cpos1,cxVec2f cpos2);

cxBezier cxBezier2Create(cxFloat time,cxVec2f epos,cxVec2f cpos1);

CX_C_END

#endif
