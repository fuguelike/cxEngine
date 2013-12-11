//
//  cxAllocator.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxUtil.h"
#include "cxAllocator.h"
#include "cxAutoPool.h"

static cxPointer cxMalloc(cxSize size)
{
    return malloc(size);
}

static cxPointer cxRealloc(cxPointer ptr,cxSize size)
{
    return realloc(ptr, size);
}

static cxPointer cxCalloc(cxSize num,cxSize size)
{
    return calloc(num, size);
}

static void cxFree(cxPointer ptr)
{
    free(ptr);
}

static cxChar *cxStrdup(cxConstChars s)
{
    cxInt len = strlen(s);
    cxChar *ret = cxMalloc(len + 1);
    memcpy(ret, s, len);
    ret[len]='\0';
    return ret;
}

const cxAllocator *allocator = &(cxAllocator){
    .malloc         = cxMalloc,
    .realloc        = cxRealloc,
    .calloc         = cxCalloc,
    .free           = cxFree,
    .strdup         = cxStrdup,
};

void cxAllocatorInit()
{
    xmlFree = cxFree;
    xmlMalloc = cxMalloc;
    xmlRealloc = cxRealloc;
    xmlMemStrdup = cxStrdup;
}

void cxAllocatorFree()
{
    
}

static void cxObjectDestroy(cxAny ptr)
{
    CX_ASSERT(ptr != NULL, "point null");
    cxObject object = (cxObject)ptr;
    object->cxFree(object);
    cxFree(ptr);
}

void cxObjectRetain(cxAny ptr)
{
    CX_RETURN(ptr == NULL);
    cxObject object = (cxObject)ptr;
    CX_ASSERT(object->cxRefcount > 0, "retain count must > 0");
    object->cxRefcount ++;
}

void cxObjectRelease(cxAny ptr)
{
    CX_RETURN(ptr == NULL);
    cxObject object = (cxObject)ptr;
    CX_ASSERT(object->cxRefcount > 0, "retain count must > 0");
    object->cxRefcount --;
    if(object->cxRefcount == 0) {
        cxObjectDestroy(ptr);
    }
}

cxAny cxObjectAutoRelease(cxAny ptr)
{
    return cxAutoPoolAppend(ptr);
}

cxAny cxObjectCreate(cxConstType type,int size,cxObjectFunc initFunc,cxObjectFunc freeFunc)
{
    cxAny any = cxObjectAlloc(type,size,initFunc,freeFunc);
    return cxObjectAutoRelease(any);
}

cxAny cxObjectAlloc(cxConstType type,int size,cxObjectFunc initFunc,cxObjectFunc freeFunc)
{
    cxAny ptr = cxCalloc(1, size);
    CX_ASSERT(ptr != NULL, "alloc memory error size=%d",size);
    cxObject object = (cxObject)ptr;
    object->cxRefcount = 1;
    object->cxType = type;
    object->cxFree = freeFunc;
    object->cxSize = size;
    initFunc(object);
    return ptr;
}








