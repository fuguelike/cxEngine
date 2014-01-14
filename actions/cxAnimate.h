//
//  cxAnimate.h
//  cxEngine
//
//  Created by xuhua on 11/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAnimate_h
#define cxEngine_cxAnimate_h

#include <core/cxArray.h>
#include <core/cxAction.h>

CX_C_BEGIN

//use at cxSprite View

void cxAnimateItemAppend(cxArray list,cxConstChars file,cxConstChars key,cxFloat delay,cxBool gcache);

CX_OBJECT_DEF(cxAnimate, cxAction)
    cxBool cached;
    cxInt index;
    cxArray list;
    cxFloat duration;
    CX_EVENT_ALLOC(onFrame);
CX_OBJECT_END(cxAnimate)

cxAnimate cxAnimateCreate(cxFloat duration,cxArray list);

CX_C_END

#endif
