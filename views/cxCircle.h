//
//  cxCircle.h
//  cxEngine
//
//  Created by xuhua on 2/2/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#ifndef cxEngine_cxCircle_h
#define cxEngine_cxCircle_h

#include "cxSprite.h"

CX_C_BEGIN

CX_DEF(cxCircle, cxSprite)
    CX_FIELD_DEF(cxFloat Percent);
    CX_FIELD_DEF(cxBool IsReverse);
    CX_FIELD_DEF(cxFloat Start);        //start angle
    cxVec3f *vs;        //pos
    cxTex2f *ts;        //texcoords
    cxColor4f *cs;      //colors
    cxInt number;       //vs number
CX_END(cxCircle, cxSprite)

CX_INLINE void cxCircleSetIsReverse(cxAny pthis,const cxBool value)
{
    CX_ASSERT_THIS(pthis, cxCircle);
    CX_RETURN(this->IsReverse == value);
    this->IsReverse = value;
    cxViewSetDirty(this, cxViewDirtyTexture);
}
CX_INLINE void cxCircleSetStart(cxAny pthis,const cxFloat degrees)
{
    CX_ASSERT_THIS(pthis, cxCircle);
    cxFloat start = fmodf(degrees + 360, 360);
    CX_RETURN(cxFloatEqu(this->Start, start));
    this->Start = start;
    cxViewSetDirty(this, cxViewDirtyTexture);
}
CX_INLINE void cxCircleSetPercent(cxAny pthis,const cxFloat value)
{
    CX_ASSERT_THIS(pthis, cxCircle);
    cxFloat percent = kmClamp(value, 0.0f, 1.0f);
    CX_RETURN(cxFloatEqu(this->Percent, percent));
    this->Percent = percent;
    cxViewSetDirty(this, cxViewDirtyTexture);
}

CX_C_END

#endif
