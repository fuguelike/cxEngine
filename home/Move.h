//
//  Move.h
//  Home
//
//  Created by xuhua on 9/22/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Move_h
#define Home_Move_h

#include <actions/cxSpline.h>

CX_C_BEGIN

//0(-d4) 30(-d3) 60(-d2) 90(d1) 120(d2) 150(d3) 180(d4) 210(d5) 240(d6) 270(d7) 300(-d6) 330(-d5)

typedef enum {
    MoveTypeNone = 0,
    MoveTypeFight,      //为作战移动
    MoveTypePosition    //为目标位置移动
}MoveType;

CX_OBJECT_DEF(Move, cxSpline)
    CX_FIELD_DEF(MoveType Type);
    CX_FIELD_DEF(cxBool IsToPoints);
CX_OBJECT_END(Move, cxSpline)

CX_FIELD_IMP(Move, cxBool, IsToPoints);
CX_FIELD_IMP(Move, MoveType, Type);

Move MoveCreate(cxAny node,cxAnyArray points);

CX_C_END

#endif
