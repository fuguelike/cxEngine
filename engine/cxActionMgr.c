//
//  cxActionMgr.c
//  cxEngine
//
//  Created by xuhua on 8/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxAction.h"
#include "cxActionMgr.h"

CX_SETTER_DEF(cxActionMgr, scale)
{
    this->scale = cxJsonToDouble(value, this->scale);
}
CX_OBJECT_TYPE(cxActionMgr, cxObject)
{
    CX_PROPERTY_SETTER(cxActionMgr, scale);
}
CX_OBJECT_INIT(cxActionMgr, cxObject)
{
    this->scale = 1.0f;
}
CX_OBJECT_FREE(cxActionMgr, cxObject)
{
    //
}
CX_OBJECT_TERM(cxActionMgr, cxObject)

cxActionMgr cxActionMgrGet(cxConstChars name)
{
    cxEngine this = cxEngineInstance();
    return cxHashGet(this->actionMgrs, cxHashStrKey(name));
}

void cxActionMgrSet(cxConstChars name,cxActionMgr mgr)
{
    cxEngine this = cxEngineInstance();
    cxHashSet(this->actionMgrs, cxHashStrKey(name), mgr);
}



