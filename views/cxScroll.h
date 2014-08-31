//
//  cxScroll.h
//  cxEngine
//
//  Created by xuhua on 11/5/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxScroll_h
#define cxEngine_cxScroll_h

#include <engine/cxView.h>

CX_C_BEGIN

typedef enum {
    cxScrollMoveTypeNone = 0,
    cxScrollMoveTypeHorizontal = 1 << 0,
    cxScrollMoveTypeVertical = 1 << 1,
}cxScrollMoveType;

CX_OBJECT_DEF(cxScroll, cxView)
    cxVec2f start;
    cxBox4f box;
    cxScrollMoveType type;
CX_OBJECT_END(cxScroll, cxView)

cxBool cxScrollTouch(cxAny pview,cxTouch *touch);

cxView cxScrollContainer(cxAny pview);

CX_C_END

#endif
