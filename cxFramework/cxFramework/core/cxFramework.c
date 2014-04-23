//
//  cxFramework.c
//  cxFramework
//
//  Created by xuhua on 4/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxFramework.h"

#if CX_TARGET_PLATFORM==CX_PLATFORM_MAC || CX_TARGET_PLATFORM==CX_PLATFORM_IOS
#include <stdio.h>
#include <libkern/OSAtomic.h>
void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap)
{
    char buffer[CX_MAX_LOGGER_LENGTH];
    vsnprintf(buffer, CX_MAX_LOGGER_LENGTH, format, ap);
    printf("[%s:%d] %s:%s\n",file,line,type,buffer);
    fflush(stdout);
}
cxUInt32 cxAtomicAddUInt32(cxUInt32 *p, cxUInt32 x)
{
    return (OSAtomicAdd32((int32_t)x, (int32_t *)p));
}
cxUInt32 cxAtomicSubUInt32(cxUInt32 *p, cxUInt32 x)
{
    return (OSAtomicAdd32(-((int32_t)x), (int32_t *)p));
}
#endif

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
    cxInt len = (cxInt)strlen(s);
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
    
}

void cxAllocatorFree()
{
    
}

void cxUtilInfo(cxConstChars file,cxInt line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("INFO", file, line, format, ap);
    va_end(ap);
}

void cxUtilError(cxConstChars file,cxInt line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("ERROR", file, line, format, ap);
    va_end(ap);
}

void cxUtilWarn(cxConstChars file,cxInt line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("WARN", file, line, format, ap);
    va_end(ap);
}

void cxUtilAssert(cxConstChars file,cxInt line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("ASSERT", file, line, format, ap);
    va_end(ap);
}

static void cxObjectDestroy(cxAny ptr)
{
    CX_ASSERT(ptr != NULL, "point null");
    cxObject object = (cxObject)ptr;
    object->cxFree(object);
    allocator->free(ptr);
}

void cxObjectRetain(cxAny ptr)
{
    CX_RETURN(ptr == NULL);
    cxObject object = (cxObject)ptr;
    CX_ASSERT(object->cxRefcount > 0, "retain count must > 0");
    cxAtomicAddUInt32(&object->cxRefcount, 1);
}

void __cxObjectAutoInit(cxObject this)
{
    
}

void __cxObjectAutoFree(cxObject this)
{
    
}

void cxObjectRelease(cxAny ptr)
{
    CX_RETURN(ptr == NULL);
    cxObject object = (cxObject)ptr;
    CX_ASSERT(object->cxRefcount > 0, "retain count must > 0");
    cxAtomicSubUInt32(&object->cxRefcount, 1);
    if(object->cxRefcount == 0) {
        cxObjectDestroy(ptr);
    }
}

cxAny cxObjectAlloc(cxConstType type,cxInt size,cxObjectFunc initFunc,cxObjectFunc freeFunc)
{
    cxAny ptr = allocator->calloc(1, size);
    CX_ASSERT(ptr != NULL && initFunc != NULL, "alloc memory error size=%d",size);
    cxObject object = (cxObject)ptr;
    object->cxRefcount = 1;
    object->cxType = type;
    object->cxFree = freeFunc;
    initFunc(object);
    return ptr;
}


void cxFrameworkInit()
{
    cxAllocatorInit();
}

void cxFrameworkFree()
{
    cxAllocatorFree();
}







