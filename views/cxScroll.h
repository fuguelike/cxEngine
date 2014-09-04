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
    cxFloat time;
    cxBool isEnable;
    cxBox4f box;
    cxScrollMoveType type;
    cxAny body;
    //scale data
    cxBool scalable;         //suppot body scale
    cxFloat scaling;
    cxRange2f range;
    cxVec2f center;
    cxFloat startDis;
CX_OBJECT_END(cxScroll, cxView)

void cxScrollUpdateBox(cxAny pview);

cxBool cxScrollTouch(cxAny pview,cxTouchItems *fires,cxTouchItems *points);

void cxScrollSetBody(cxAny pview,cxAny cview);

cxView cxScrollBody(cxAny pview);

cxBool cxScrollCheckPos(cxScroll this,cxVec2f *pos);

CX_C_END

#endif
