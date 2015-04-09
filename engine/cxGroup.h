//
//  cxGroup.h
//  cxengine
//  cxAction group mgr
//  Created by xuhua on 8/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEingine_cxGroup_h
#define cxEingine_cxGroup_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

CX_DEF(cxGroup, cxObject)
    CX_FIELD_DEF(cxFloat Scale);
    CX_FIELD_DEF(cxBool IsPause);
CX_END(cxGroup, cxObject)

CX_FIELD_GET(cxGroup, cxFloat, Scale);
CX_FIELD_GET(cxGroup, cxBool, IsPause);

void cxGroupFireUpdate(cxAny pg);

cxGroup cxGroupGet(cxConstChars name);

void cxGroupSet(cxConstChars name,cxGroup mgr);

cxGroup cxGroupAppend(cxConstChars name, cxFloat scale);

void cxGroupSetScale(cxConstChars name,cxFloat scale);

void cxGroupSetPause(cxConstChars name,cxBool pause);

CX_INLINE void cxGroupReset(cxConstChars name)
{
    cxGroup this = cxGroupGet(name);
    this->Scale = 1.0f;
    this->IsPause = false;
}

CX_C_END

#endif
