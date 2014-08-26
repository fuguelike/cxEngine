//
//  cxAnimate.h
//  cxEngine
//
//  Created by xuhua on 11/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAnimate_h
#define cxEngine_cxAnimate_h

#include <engine/cxAction.h>

CX_C_BEGIN

//use at cxSprite View

void cxAnimateItemAppend(cxArray list,cxConstChars file,cxConstChars keyfmt,cxFloat delay);

void cxAnimateAppendSeries(cxArray list,cxConstChars file,cxConstChars keyfmt,cxInt from,cxInt to,cxFloat delay);

CX_OBJECT_DEF(cxAnimate, cxAction)
    cxBool cached;
    cxInt index;
    cxArray list;
    cxFloat time;
    CX_EVENT_ALLOC(onFrame);
CX_OBJECT_END(cxAnimate, cxAction)

cxAnimate cxAnimateCreate(cxFloat time,cxArray list);

CX_C_END

#endif
