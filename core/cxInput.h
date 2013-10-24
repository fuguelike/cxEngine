//
//  cxInput.h
//  cxEngine
//
//  Created by xuhua on 10/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxInput_h
#define cxEngine_cxInput_h

#include "cxTypes.h"
#include "cxBase.h"

CX_C_BEGIN

typedef enum {
    cxTouchTypeNone = 0,
    cxTouchTypeDown,
    cxTouchTypeMove,
    cxTouchTypeUp,
    cxTouchTypeCancel,
}cxTouchType;

typedef struct {
    cxVec2f previous;
    cxVec2f delta;
    cxVec2f current;
    cxTouchType type;
    cxVec2f movement;
    cxDouble dtime;
    cxDouble utime;
}cxTouch;

CX_C_END

#endif
