//
//  cxFade.h
//  cxEngine
//  透明度
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFade_h
#define cxEngine_cxFade_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxFade, cxAction)
    cxFloat alpha;
    cxFloat delta;
    cxFloat start;
CX_OBJECT_END(cxFade, cxAction)

cxFade cxFadeCreate(cxFloat time,cxFloat alpha);

CX_C_END

#endif
