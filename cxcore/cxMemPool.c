//
//  cxMemPool.c
//  cxCore
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <pthread.h>
#include "cxMemPool.h"
#include "cxStack.h"

static pthread_key_t autoPoolKey;

CX_OBJECT_TYPE(cxMemPool, cxObject)
{}
CX_OBJECT_INIT(cxMemPool, cxObject)
{
    this->objects = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxMemPool, cxObject)
{
    CX_RELEASE(this->objects);
}
CX_OBJECT_TERM(cxMemPool, cxObject)

static cxStack cxMemPoolStack()
{
    cxAny pool = pthread_getspecific(autoPoolKey);
    if(pool == NULL){
        pool = CX_ALLOC(cxStack);
        pthread_setspecific(autoPoolKey, pool);
    }
    return pool;
}

void cxMemPoolInit()
{
    pthread_key_create(&autoPoolKey, __cxObjectRelease);
}

void cxMemPoolFree()
{
    cxMemPoolClear();
    pthread_key_delete(autoPoolKey);
}

static cxMemPool cxMemPoolInstance()
{
    cxMemPool pool = NULL;
    cxStack stack = cxMemPoolStack();
    if(cxStackLength(stack) == 0){
        pool = CX_ALLOC(cxMemPool);
        cxStackPush(stack, pool);
        CX_RELEASE(pool);
    }else{
        pool = cxStackTop(stack);
    }
    return pool;
}

cxAny cxMemPoolAppend(cxAny any)
{
    CX_RETURN(any == NULL,NULL);
    cxObject object = any;
    cxMemPool pool = cxMemPoolInstance();
    cxArrayAppend(pool->objects, any);
    CX_ASSERT(object->cxRefcount > 1, "apped to auto pool object,refcount > 1");
    CX_UNUSED_PARAM(object);
    CX_RELEASE(any);
    return any;
}

void cxMemPoolBegin()
{
    cxMemPoolInstance();
}

void cxMemPoolClear()
{
    cxMemPool pool = cxMemPoolInstance();
    cxArrayClear(pool->objects);
}

void cxMemPoolPush()
{
    cxMemPool pool = CX_ALLOC(cxMemPool);
    cxStack stack = cxMemPoolStack();
    cxStackPush(stack, pool);
    CX_RELEASE(pool);
}

void cxMemPoolPop()
{
    cxStack stack = cxMemPoolStack();
    cxStackPop(stack);
}






