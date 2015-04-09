//
//  cxClipping.h
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxClipping_h
#define cxEngine_cxClipping_h

#include <engine/cxView.h>

CX_C_BEGIN

CX_DEF(cxClipping, cxView)
    cxUInt useRef;
    cxBool inverse;
CX_END(cxClipping, cxView)

cxTexture cxClippingTexture(cxAny pview);

void cxClippingSetInverse(cxAny pview,cxBool inverse);

CX_C_END

#endif
