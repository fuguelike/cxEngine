//
//  cxScroll.h
//  cxEngine
//
//  Created by xuhua on 11/5/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxScroll_h
#define cxEngine_cxScroll_h

#include <core/cxView.h>

CX_C_BEGIN

typedef enum {
    cxScrollMoveTypeNone = 0,
    cxScrollMoveTypeHorizontal = 1 << 0,
    cxScrollMoveTypeVertical = 1 << 1,
}cxScrollMoveType;

CX_OBJECT_DEF(cxScroll, cxView)
    cxFloat max;
    cxFloat value;
    cxBox4f box;
    cxScrollMoveType type;
    cxBool selected;
    CX_SLOT_ALLOC(onTouch);
CX_OBJECT_END(cxScroll)

void cxScrollInitView(cxAny pview,cxJson json);

cxBool cxScrollTouch(cxAny pview,cxTouch *touch);

cxView cxScrollContainer(cxAny pview);

CX_C_END

#endif
