//
//  cxAutoPool.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxAutoPool.h"
#include "cxStack.h"

cxStack autoPool = NULL;

CX_OBJECT_INIT(cxAutoPool, cxObject)
{
    this->objects = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxAutoPool, cxObject)
{
    CX_RELEASE(this->objects);
}
CX_OBJECT_TERM(cxAutoPool, cxObject)

static cxStack cxAutoPoolStack()
{
    return autoPool;
}

void cxAutoPoolInit()
{
    autoPool = CX_ALLOC(cxStack);
}

void cxAutoPoolFree()
{
    cxAutoPoolClean();
    CX_RELEASE(autoPool);
}

static cxAutoPool cxAutoPoolInstance()
{
    cxAutoPool pool = NULL;
    cxStack stack = cxAutoPoolStack();
    if(cxStackLength(stack) == 0){
        pool = CX_ALLOC(cxAutoPool);
        cxStackPush(stack, pool);
        CX_RELEASE(pool);
    }else{
        pool = cxStackTop(stack);
    }
    return pool;
}

cxAny cxAutoPoolAppend(cxAny any)
{
    CX_RETURN(any == NULL,NULL);
    cxObject object = any;
    cxAutoPool pool = cxAutoPoolInstance();
    cxArrayAppend(pool->objects, any);
    CX_ASSERT(object->cxRefcount > 1, "apped to auto pool object,refcount > 1");
    CX_UNUSED_PARAM(object);
    CX_RELEASE(any);
    return any;
}

void cxAutoPoolBegin()
{
    cxAutoPoolInstance();
}

void cxAutoPoolClean()
{
    cxAutoPool pool = cxAutoPoolInstance();
    cxArrayClean(pool->objects);
}

void cxAutoPoolPush()
{
    cxAutoPool pool = CX_ALLOC(cxAutoPool);
    cxStack stack = cxAutoPoolStack();
    cxStackPush(stack, pool);
    CX_RELEASE(pool);
}

void cxAutoPoolPop()
{
    cxStack stack = cxAutoPoolStack();
    cxStackPop(stack);
}






