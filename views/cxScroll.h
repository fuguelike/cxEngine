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

CX_OBJECT_DEF(cxScroll, cxView)
    cxBool enable;
CX_OBJECT_END(cxScroll)

void cxScrollUpdateSubviewsPos(cxAny pview,cxVec2f delta);

cxBool cxScrollTouch(cxAny pview,cxTouch *touch);

CX_C_END

#endif
