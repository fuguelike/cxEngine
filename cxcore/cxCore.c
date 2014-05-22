//
//  cxFramework.c
//  cxFramework
//
//  Created by xuhua on 4/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxCore.h"
#include "cxType.h"
#include "cxMemPool.h"

//MAC atomic support
#if CX_TARGET_PLATFORM==CX_PLATFORM_MAC
#include <libkern/OSAtomic.h>
cxUInt32 cxAtomicAddInt32(cxInt32 *p, cxInt32 x)
{
    return OSAtomicAdd32((int32_t)x, (int32_t *)p);
}
cxUInt32 cxAtomicSubInt32(cxInt32 *p, cxInt32 x)
{
    return OSAtomicAdd32(-((int32_t)x), (int32_t *)p);
}
//IOS atomic support
#elif CX_TARGET_PLATFORM == CX_PLATFORM_IOS
#include <libkern/OSAtomic.h>
cxUInt32 cxAtomicAddInt32(cxInt32 *p, cxInt32 x)
{
    return OSAtomicAdd32((int32_t)x, (int32_t *)p);
}
cxUInt32 cxAtomicSubInt32(cxInt32 *p, cxInt32 x)
{
    return OSAtomicAdd32(-((int32_t)x), (int32_t *)p);
}
//ANDROID atomic support
#elif CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID
#include <sys/atomics.h>
cxUInt32 cxAtomicAddInt32(cxInt32 *p, cxInt32 x)
{
    return __sync_fetch_and_add(p,x);
}
cxUInt32 cxAtomicSubInt32(cxInt32 *p, cxInt32 x)
{
    return __sync_fetch_and_sub(p,x);
}
#endif

static cxPointer cxMalloc(cxSize size)
{
    return calloc(1,size);
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
    cxMemPoolInit();
}

void cxAllocatorFree()
{
    cxMemPoolFree();
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
    cxAtomicAddInt32(&object->cxRefcount, 1);
}

cxAny cxObjectType(cxAny object)
{
    CX_RETURN(object == NULL, NULL);
    cxObject this = object;
    return cxTypesGet(this->cxType);
}

cxAny cxObjectProperty(cxAny object,cxConstChars key)
{
    CX_RETURN(object == NULL, NULL);
    cxType type = cxObjectType(object);
    return cxTypeProperty(type, key);
}

void __cxObjectAutoType(cxAny this)
{
    
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
    CX_ASSERT(object->cxRefcount > 0, "error,retain count must > 0");
    cxAtomicSubInt32(&object->cxRefcount, 1);
    if(object->cxRefcount == 0) {
        cxObjectDestroy(ptr);
    }
}

cxAny cxObjectAutoRelease(cxAny ptr)
{
    return cxMemPoolAppend(ptr);
}

cxAny cxObjectCreate(cxConstType type, int size,cxObjectFunc initFunc,cxObjectFunc freeFunc)
{
    cxAny any = cxObjectAlloc(type, size, initFunc, freeFunc);
    return cxObjectAutoRelease(any);
}

cxAny cxObjectAlloc(cxConstType type,cxInt size,cxObjectFunc initFunc,cxObjectFunc freeFunc)
{
    CX_ASSERT(initFunc != NULL, "init func null");
    cxAny ptr = allocator->malloc(size);
    CX_ASSERT(ptr != NULL, "alloc memory error size=%d",size);
    cxObject object = (cxObject)ptr;
    object->cxRefcount = 1;
    object->cxType = type;
    object->cxFree = freeFunc;
    initFunc(object);
    return ptr;
}

void cxCoreInit()
{
    cxAllocatorInit();
    cxTypesInit();
}

void cxCoreFree()
{
    cxTypesFree();
    cxAllocatorFree();
}







