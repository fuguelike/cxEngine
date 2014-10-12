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

CX_OBJECT_DEF(cxGroup, cxObject)
    CX_FIELD_DEF(cxFloat Scale);
CX_OBJECT_END(cxGroup, cxObject)

CX_FIELD_IMP(cxGroup, cxFloat, Scale);

cxGroup cxGroupGet(cxConstChars name);

void cxGroupSet(cxConstChars name,cxGroup mgr);

void cxGroupAppend(cxConstChars name, cxFloat scale);

void cxGroupSetTimeScale(cxConstChars name,cxFloat scale);

CX_C_END

#endif
