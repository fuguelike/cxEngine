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
CX_OBJECT_TYPE(cxGroup, cxObject)
{
    CX_PROPERTY_SETTER(cxGroup, scale);
}
CX_OBJECT_INIT(cxGroup, cxObject)
{
    this->Scale = 1.0f;
}
CX_OBJECT_FREE(cxGroup, cxObject)
{
    //
}
CX_OBJECT_TERM(cxGroup, cxObject)

cxGroup cxGroupGet(cxConstChars name)
{
    cxEngine this = cxEngineInstance();
    return cxHashGet(this->groups, cxHashStrKey(name));
}

void cxGroupSet(cxConstChars name,cxGroup mgr)
{
    cxEngine this = cxEngineInstance();
    cxHashSet(this->groups, cxHashStrKey(name), mgr);
}



