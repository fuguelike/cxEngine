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
    cxScrollMoveTypeNone        = 0,
    cxScrollMoveTypeHorizontal  = 1 << 0,
    cxScrollMoveTypeVertical    = 1 << 1,
}cxScrollMoveType;

CX_OBJECT_DEF(cxScroll, cxView)
    //move data
    cxFloat speed;
    cxFloat moveTime;
    cxBool isEnable;
    cxBox4f box;
    cxScrollMoveType type;
    //can move view
    CX_FIELD_DEF(cxAny Body);
    //scale data
    cxFloat scaleTime;
    cxFloat scaleinc;           //increase scale
    cxBool scalable;            //suppot body scale
    cxFloat scaling;
    cxRange2f range;
    cxVec2f center;
    cxFloat startDis;
CX_OBJECT_END(cxScroll, cxView)

CX_FIELD_GET(cxScroll, cxAny, Body);
void cxScrollSetBody(cxAny pview,cxAny body);

void cxScrollUpdateBox(cxAny pview);

cxBool cxScrollTouch(cxAny pview,const cxTouchItems *points);

void cxScrollSetEnable(cxAny pview,cxBool enable);

cxBool cxScrollCheckPos(cxAny pview,cxVec2f *pos);

CX_C_END

#endif
