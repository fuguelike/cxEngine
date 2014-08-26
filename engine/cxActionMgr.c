//
//  cxActionMgr.c
//  cxEngine
//
//  Created by xuhua on 8/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxAction.h"
#include "cxActionMgr.h"

CX_OBJECT_TYPE(cxActionMgr, cxObject)
{
    
}
CX_OBJECT_INIT(cxActionMgr, cxObject)
{
    this->scale = 1.0f;
}
CX_OBJECT_FREE(cxActionMgr, cxObject)
{

}
CX_OBJECT_TERM(cxActionMgr, cxObject)

cxActionMgr cxActionMgrCreate()
{
    cxActionMgr this = CX_CREATE(cxActionMgr);
    return this;
}

cxAny cxActionMgrTimer(cxActionMgr this,cxAny pview,cxFloat freq,cxInt repeat)
{
    CX_ASSERT(this != NULL && pview != NULL, "args empty");
    cxAny timer = cxViewAppendTimer(pview, freq, repeat);
    cxActionSetMgr(timer, this);
    return timer;
}

void cxActionMgrAppend(cxActionMgr this,cxAny pav, cxAny pview)
{
    CX_ASSERT(this != NULL && pav != NULL && pview != NULL, "args empty");
    cxActionSetMgr(pav, this);
    cxViewAppendAction(pview, pav);
}

