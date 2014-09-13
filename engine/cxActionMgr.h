//
//  cxActionMgr.h
//  cxengine
//  cxAction group mgr
//  Created by xuhua on 8/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEingine_cxActionMgr_h
#define cxEingine_cxActionMgr_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxActionMgr, cxObject)
    cxFloat scale;
CX_OBJECT_END(cxActionMgr, cxObject)

cxActionMgr cxActionMgrGet(cxConstChars name);

void cxActionMgrSet(cxConstChars name,cxActionMgr mgr);

CX_C_END

#endif
