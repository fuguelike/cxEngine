//
//  cxHash.h
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxHash_h
#define cxEngine_cxHash_h

#include <uthash.h>
#include "cxBase.h"

CX_C_BEGIN

#define CX_HASH_MAX_KEY_LENGTH  128

#define CX_HASH_FOREACH(hash,ele,tmp)  cxHashElement *ele = NULL,*tmp=NULL;HASH_ITER(hh, hash->hashPtr, ele, tmp)

typedef struct {
    cxAny any;
    cxChar key[CX_HASH_MAX_KEY_LENGTH+1];
    UT_hash_handle hh;
}cxHashElement;

typedef struct {
    cxInt length;
    cxChar *data;
}cxHashKey;

typedef int (*cxHashCmpFunc)(cxHashElement *lp,cxHashElement *rp);

#define cxHashIntKey(v)      (cxHashKey){.length=sizeof(cxInt),.data=(cxChar *)&(cxInt){v}}

#define cxHashStrKey(s)      (cxHashKey){.length=(cxInt)strlen(s),.data=(cxChar *)s}

#define cxHashMemKey(d,l)    (cxHashKey){.length=l,.data=(cxChar *)d}

CX_OBJECT_DEF(cxHash, cxObject)
    cxHashElement *hashPtr;
CX_OBJECT_END(cxHash)

#define cxHashSort(hash,cmp)    HASH_SORT(hash,cmp)

void    cxHashClean(cxHash hash);

cxInt   cxHashLength(cxHash hash);

void    cxHashDelElement(cxHash hash,cxHashElement *element);

cxHashElement *cxHashGetElement(cxHash hash,cxHashKey key);

cxBool  cxHashHas(cxHash hash,cxHashKey key);

void    cxHashDel(cxHash hash,cxHashKey key);

void    cxHashSet(cxHash hash,cxHashKey key,cxAny any);

cxAny   cxHashGet(cxHash hash,cxHashKey key);

CX_C_END

#endif























