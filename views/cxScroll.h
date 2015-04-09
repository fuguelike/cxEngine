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
    cxScrollMoveTypeLeft        = 1 << 2,
    cxScrollMoveTypeRight       = 1 << 3,
    cxScrollMoveTypeTop         = 1 << 4,
    cxScrollMoveTypeBottom      = 1 << 5,
}cxScrollMoveType;

CX_DEF(cxScroll, cxView)
    CX_FIELD_DEF(cxInt BodyIdx);  //body at subviews idx
    //move data
    cxFloat speed;
    cxFloat moveTime;
    cxBool isEnable;
    cxBox4f box;
    cxScrollMoveType type;
    //scale data
    cxFloat scaleTime;
    cxFloat scaleinc;           //increase scale
    cxBool scalable;            //suppot body scale
    cxFloat scaling;
    cxRange2f range;
    cxFloat startDis;
    cxVec2f location;           //location pos
CX_END(cxScroll, cxView)

CX_FIELD_SET(cxScroll, cxInt, BodyIdx);
CX_INLINE cxAny cxScrollGetBody(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxScroll);
    cxList subviews = cxViewGetSubViews(this);
    CX_ASSERT(subviews != NULL, "scroll body miss");
    return cxListAt(subviews, this->BodyIdx);
}
CX_INLINE cxVec2f cxScrollGetScale(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxScroll);
    cxAny body = cxScrollGetBody(this);
    return cxViewGetScale(body);
}

void cxScrollUpdateBox(cxAny pview);

void cxScrollUpdate(cxAny pview);

cxBool cxScrollCheckPos(cxAny pview,cxVec2f *pos);

void cxScrollLocationTo(cxAny pview,cxVec2f pos,cxBool animate);

CX_C_END

#endif
