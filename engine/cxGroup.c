//
//  cxGroup.c
//  cxEngine
//
//  Created by xuhua on 8/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxAction.h"
#include "cxGroup.h"

CX_SETTER_DEF(cxGroup, scale)
{
    this->Scale = cxJsonToDouble(value, this->Scale);
}
static void CX_METHOD(cxGroup, Update,cxAny pav)
{
    
}

CX_TYPE(cxGroup, cxObject)
{
    CX_SETTER(cxGroup, scale);
    CX_MSET(cxGroup, Update);
}
CX_INIT(cxGroup, cxObject)
{
    this->Scale = 1.0f;
}
CX_FREE(cxGroup, cxObject)
{
    //
}
CX_TERM(cxGroup, cxObject)

cxFloat cxGroupGetScale(cxAny group,cxAny pav)
{
    CX_ASSERT_THIS(group, cxGroup);
    CX_CALL(this, Update, CX_MT(void,cxAny), pav);
    return this->Scale;
}

cxGroup cxGroupGet(cxConstChars name)
{
    cxEngine this = cxEngineInstance();
    return cxHashGet(this->groups, cxHashStrKey(name));
}

void cxGroupSetTimeScale(cxConstChars name,cxFloat scale)
{
    cxGroup group = cxGroupGet(name);
    cxGroupSetScale(group, scale);
}

cxGroup cxGroupAppend(cxConstChars name, cxFloat scale)
{
    cxGroup group = CX_CREATE(cxGroup);
    cxGroupSetScale(group, scale);
    cxGroupSet(name, group);
    return group;
}

void cxGroupSet(cxConstChars name,cxGroup mgr)
{
    cxEngine this = cxEngineInstance();
    cxHashSet(this->groups, cxHashStrKey(name), mgr);
}



