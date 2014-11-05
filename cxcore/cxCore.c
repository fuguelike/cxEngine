//
//  cxFramework.c
//  cxFramework
//
//  Created by xuhua on 4/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <sys/time.h>
#include "cxCore.h"
#include "cxType.h"
#include "cxMemPool.h"
#include "cxStack.h"
#include "cxMessage.h"

#if CX_TARGET_PLATFORM == CX_PLATFORM_IOS
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

static cxAny cxMalloc(cxSize size)
{
    return calloc(1,size);
}

static cxAny cxRealloc(cxAny ptr,cxSize size)
{
    return realloc(ptr, size);
}

static cxAny cxCalloc(cxInt num,cxSize size)
{
    return calloc(num, size);
}

static void cxFree(cxAny ptr)
{
    free(ptr);
}

static cxChars cxStrdup(cxConstChars s)
{
    cxInt len = (cxInt)strlen(s);
    cxChars ret = cxMalloc(len + 1);
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

static void __cxObjectDestroy(cxAny ptr)
{
    CX_ASSERT(ptr != NULL, "point null");
    cxObject object = (cxObject)ptr;
    object->cxFree(object);
    allocator->free(ptr);
}

void __cxObjectRetain(cxAny ptr)
{
    CX_RETURN(ptr == NULL);
    cxObject object = (cxObject)ptr;
    CX_ASSERT(object->cxRefcount > 0, "retain count must > 0");
    cxAtomicAddInt32(&object->cxRefcount, 1);
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

void __cxObjectRelease(cxAny ptr)
{
    CX_RETURN(ptr == NULL);
    cxObject object = (cxObject)ptr;
    CX_ASSERT(object->cxRefcount > 0, "error,retain count must > 0");
    cxAtomicSubInt32(&object->cxRefcount, 1);
    if(object->cxRefcount == 0) {
        __cxObjectDestroy(ptr);
    }
}

cxAny __cxObjectAutoRelease(cxAny ptr)
{
    return cxMemPoolAppend(ptr);
}

cxAny __cxObjectCreate(cxConstType type, int size,cxObjectFunc initFunc,cxObjectFunc freeFunc)
{
    cxAny any = __cxObjectAlloc(type, size, initFunc, freeFunc);
    return __cxObjectAutoRelease(any);
}

cxAny __cxObjectAlloc(cxConstType type,cxInt size,cxObjectFunc initFunc,cxObjectFunc freeFunc)
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

void __cxTypeRegisterType(cxConstType tt,cxConstType bb,cxAny (*create)(),cxAny (*alloc)(),void (*autoType)(cxAny))
{
    CX_ASSERT(autoType != NULL, "auto type func error");
    cxType type = CX_ALLOC(cxType);
    cxType superType = cxTypesGet(bb);
    //cxObject have not super type
    CX_ASSERT(superType != NULL || tt == cxObjectTypeName,"type %s not register",bb);
    cxTypesSet(tt,type);
    cxTypeSetSuper(type,superType);
    cxTypeSignature(type,superType);
    type->Alloc = alloc;
    type->Create = create;
    autoType(type);
    CX_RELEASE(type);
}

cxDouble cxTimestamp()
{
    struct timeval val = {0};
    gettimeofday(&val, NULL);
    return val.tv_sec + (cxDouble)val.tv_usec/(cxDouble)1000000.0;
}

static cxHash groups = NULL;

void cxCorePush(cxCoreStackType type,cxAny object)
{
    CX_ASSERT(groups != NULL, "global stack null");
    cxHashKey key = cxHashIntKey(type);
    cxAny stack = cxHashGet(groups, key);
    if(stack == NULL){
        stack = CX_ALLOC(cxStack);
        cxHashSet(groups, key, stack);
        CX_RELEASE(stack);
    }
    cxStackPush(stack, object);
}

cxAny cxCoreTop(cxCoreStackType type)
{
    CX_ASSERT(groups != NULL, "global stack null");
    cxAny stack = cxHashGet(groups, cxHashIntKey(type));
    return stack != NULL ? cxStackTop(stack) : NULL;
}

void cxCorePop(cxCoreStackType type)
{
    CX_ASSERT(groups != NULL, "global stack null");
    cxAny stack = cxHashGet(groups, cxHashIntKey(type));
    if(stack != NULL){
        cxStackPop(stack);
    }
}

void cxCoreInit()
{
    cxAllocatorInit();
    cxTypesInit();
    groups = CX_ALLOC(cxHash);
    cxMessageInstance();
}

void cxCoreFree()
{
    CX_RETURN(groups == NULL);
    cxMessageDestroy();
    CX_RELEASE(groups);
    cxTypesFree();
    cxAllocatorFree();
    groups = NULL;
}







