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
    CX_METHOD_DEF(void, onUpdate,cxAny group,cxAny pav);
CX_END(cxGroup, cxObject)

CX_FIELD_SET(cxGroup, cxFloat, Scale);
cxFloat cxGroupGetScale(cxAny group,cxAny pav);

cxGroup cxGroupGet(cxConstChars name);

void cxGroupSet(cxConstChars name,cxGroup mgr);

cxGroup cxGroupAppend(cxConstChars name, cxFloat scale);

void cxGroupSetTimeScale(cxConstChars name,cxFloat scale);

CX_C_END

#endif
