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

CX_OBJECT_DEF(cxAnimateItem, cxObject)
    cxTexture texture;
    cxString key;
    cxFloat time;
    cxFloat value;
CX_OBJECT_END(cxAnimateItem, cxObject)

CX_OBJECT_DEF(cxAnimate, cxAction)
    cxHash groups;
    cxHash frames;
    cxInt index;
    cxFloat time;
    cxBool forever;
    cxString key;
    CX_EVENT_ALLOC(onFrame);
CX_OBJECT_END(cxAnimate, cxAction)

cxAnimateItem cxAnimateGetFrame(cxAny pav,cxConstChars key);

cxHash cxAnimateGetGroup(cxAny pav,cxString key);

void cxAnimateSetKey(cxAny pav,cxConstChars name);

cxAnimate cxAnimateCreate(cxFloat time,cxConstChars key);

CX_C_END

#endif
