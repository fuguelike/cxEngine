//
//  cxAllocator.h
//  cxEngine
//  内存管理和分配
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAllocator_h
#define cxEngine_cxAllocator_h

#include "cxConfig.h"

CX_C_BEGIN

typedef cxPointer (*cxMallocFunc)(cxSize size);

typedef cxPointer (*cxReallocFunc)(cxPointer ptr,cxSize size);

typedef cxPointer (*cxCallocFunc)(cxSize num,cxSize size);

typedef void (*cxFreeFunc)(cxPointer ptr);

typedef cxChar *(*cxStrdupFunc)(cxConstChars s);

typedef struct {
    cxMallocFunc    malloc;
    cxReallocFunc   realloc;
    cxCallocFunc    calloc;
    cxFreeFunc      free;
    cxStrdupFunc    strdup;
}cxAllocator;

extern const cxAllocator *allocator;

void cxAllocatorInit();

void cxAllocatorFree();

cxAny cxObjectAlloc(cxConstType type,int size,cxObjectFunc initFunc,cxObjectFunc freeFunc);

cxAny cxObjectCreate(cxConstType type,int size,cxObjectFunc initFunc,cxObjectFunc freeFunc);

void cxObjectRetain(cxAny ptr);

void cxObjectRelease(cxAny ptr);

cxAny cxObjectAutoRelease(cxAny ptr);

CX_C_END

#endif
