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

#define CX_ANIMATE_DEFAULT_GROUP "__DefaultGroupName__"

CX_OBJECT_DEF(cxAnimate, cxAction)
    cxString groupName;
    cxHash groups;
    cxInt index;
    cxFloat time;
    cxBool forever;
    CX_EVENT_ALLOC(onFrame);
CX_OBJECT_END(cxAnimate, cxAction)

cxAnimateItem cxAnimateItemAt(cxAny pav,cxInt idx);

cxArray cxAnimateGroup(cxAny pav,cxConstChars name);

void cxAnimateSetGroupName(cxAny pav,cxConstChars name);

cxAnimate cxAnimateCreate(cxFloat time,cxConstChars name);

CX_C_END

#endif
