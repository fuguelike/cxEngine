//
//  cxValue.c
//  cxCore
//
//  Created by xuhua on 10/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxValue.h"

static cxBool cxValueEqu(cxAny pobj)
{
    CX_ASSERT_THIS(pobj, cxValue);
    return memcmp(this->newVar, this->oldVar, this->size) == 0;
}

static void cxValueUpdate(cxAny pobj,cxFloat dt)
{
    CX_ASSERT_THIS(pobj, cxValue);
    if(CX_METHOD_GET(false, this->ValueEqu, this)){
        return;
    }
    CX_EVENT_FIRE(this, onChanged);
    memcpy(this->oldVar, this->newVar, this->size);
}

CX_OBJECT_TYPE(cxValue, cxObject)
{
    
}
CX_OBJECT_INIT(cxValue, cxObject)
{
    cxEngine engine = cxEngineInstance();
    CX_LIN(cxEngine, engine, onUpdate, this, cxValueUpdate);
    CX_SET(cxValue, this, ValueEqu, cxValueEqu);
}
CX_OBJECT_FREE(cxValue, cxObject)
{
    CX_SLOT_RELEASE(this->onUpdate);
    allocator->free(this->oldVar);
    allocator->free(this->newVar);
    CX_EVENT_RELEASE(this->onChanged);
}
CX_OBJECT_TERM(cxValue, cxObject)

cxValue cxValueAllocImp(cxInt size)
{
    CX_ASSERT(size > 0, "size error");
    cxValue this = CX_ALLOC(cxValue);
    this->size = size;
    this->newVar = allocator->malloc(size);
    this->oldVar = allocator->malloc(size);
    return this;
}

cxValue cxValueCreateImp(cxInt size)
{
    return CX_AUTO(cxValueAllocImp(size));
}