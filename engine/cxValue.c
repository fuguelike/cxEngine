//
//  cxValue.c
//  cxCore
//
//  Created by xuhua on 10/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxValue.h"

static void cxValueUpdate(cxAny pobj,cxFloat dt)
{
    CX_ASSERT_THIS(pobj, cxValue);
    CX_RETURN(!this->isChanged);
    memcpy(this->oldVar, this->newVar, this->size);
    CX_EVENT_FIRE(this, onChanged);
    this->isChanged = false;
}

void cxValueSet(cxAny pthis,cxAny v)
{
    CX_ASSERT_THIS(pthis, cxValue);
    memcpy(this->newVar,v,this->size);
    this->isChanged = CX_CALL(this, Equ, CX_MT(cxBool));
}

static cxBool CX_METHOD(cxValue, Equ)
{
    return memcmp(this->newVar, this->oldVar, this->size) == 0;
}

CX_TYPE(cxValue, cxObject)
{
    CX_MSET(cxValue, Equ);
}
CX_INIT(cxValue, cxObject)
{
    cxEngine engine = cxEngineInstance();
    CX_CON(cxEngine, engine, onUpdate, this, cxValueUpdate);
}
CX_FREE(cxValue, cxObject)
{
    CX_SLOT_RELEASE(this->onUpdate);
    CX_EVENT_RELEASE(this->onChanged);
}
CX_TERM(cxValue, cxObject)

cxValue cxValueAllocImp(cxInt size)
{
    CX_ASSERT(size > 0 && size < CX_VALUE_MAX_SIZE, "size error,please inc CX_VALUE_MAX_SIZE");
    cxValue this = CX_ALLOC(cxValue);
    this->size = size;
    return this;
}

cxValue cxValueCreateImp(cxInt size)
{
    return CX_AUTO(cxValueAllocImp(size));
}