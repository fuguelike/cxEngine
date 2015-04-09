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
CX_SETTER_DEF(cxGroup, pause)
{
    this->IsPause = cxJsonToBool(value, this->IsPause);
}
CX_TYPE(cxGroup, cxObject)
{
    CX_SETTER(cxGroup, pause);
    CX_SETTER(cxGroup, scale);
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

void cxGroupFireUpdate(cxAny pg)
{
    CX_ASSERT_THIS(pg, cxGroup);
    CX_CALL(this, Update, CX_M(void));
}

cxGroup cxGroupGet(cxConstChars name)
{
    cxEngine this = cxEngineInstance();
    return cxHashGet(this->groups, cxHashStrKey(name));
}

void cxGroupSetScale(cxConstChars name,cxFloat scale)
{
    cxGroup group = cxGroupGet(name);
    CX_ASSERT(group != NULL, "%s action group not exists",name);
    group->Scale = scale;
}

void cxGroupSetPause(cxConstChars name,cxBool pause)
{
    cxGroup group = cxGroupGet(name);
    CX_ASSERT(group != NULL, "%s action group not exists",name);
    group->IsPause = pause;
}

cxGroup cxGroupAppend(cxConstChars name, cxFloat scale)
{
    cxGroup group = CX_CREATE(cxGroup);
    group->Scale = scale;
    cxGroupSet(name, group);
    return group;
}

void cxGroupSet(cxConstChars name,cxGroup mgr)
{
    cxEngine this = cxEngineInstance();
    cxHashSet(this->groups, cxHashStrKey(name), mgr);
}



