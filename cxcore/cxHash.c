//
//  cxHash.c
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxHash.h"

CX_OBJECT_TYPE(cxHash, cxObject)
{}
CX_OBJECT_INIT(cxHash, cxObject)
{}
CX_OBJECT_FREE(cxHash, cxObject)
{
    cxHashClean(this);
}
CX_OBJECT_TERM(cxHash, cxObject)

#define CX_HASH_KEY_OK(_k_) ((_k_).data != NULL && (_k_).length > 0 && (_k_).length < CX_HASH_MAX_KEY)

void cxHashClean(cxHash hash)
{
    cxHashElement *ele = NULL, *tmp = NULL;
    HASH_ITER(hh, hash->hashPtr, ele, tmp){
        CX_RELEASE(ele->any);
    }
    HASH_CLEAR(hh, hash->hashPtr);
}

void cxHashDelElement(cxHash hash,cxHashElement *element)
{
    HASH_DEL(hash->hashPtr, element);
    CX_RELEASE(element->any);
    allocator->free(element);
}

cxBool cxHashHas(cxHash hash,cxHashKey key)
{
    CX_ASSERT(CX_HASH_KEY_OK(key), "hash key error");
    cxHashElement *element = NULL;
    HASH_FIND(hh, hash->hashPtr, key.data, key.length, element);
    return element != NULL;
}

cxBool cxHashDel(cxHash hash,cxHashKey key)
{
    cxHashElement *element = NULL;
    HASH_FIND(hh, hash->hashPtr, key.data, key.length, element);
    if(element != NULL){
        cxHashDelElement(hash, element);
        return true;
    }
    return false;
}

cxInt cxHashLength(cxHash hash)
{
    return HASH_COUNT(hash->hashPtr);
}

cxAny cxHashFirst(cxHash hash)
{
    cxHashElement *ele = NULL, *tmp = NULL;
    HASH_ITER(hh, hash->hashPtr, ele, tmp){
        return ele->any;
    }
    return NULL;
}

cxAny cxHashGet(cxHash hash,cxHashKey key)
{
    cxHashElement *element = cxHashGetElement(hash, key);
    return element == NULL ? NULL : element->any;
}

cxHashElement *cxHashGetElement(cxHash hash,cxHashKey key)
{
    CX_ASSERT(CX_HASH_KEY_OK(key), "hash key error");
    cxHashElement *element = NULL;
    HASH_FIND(hh, hash->hashPtr, key.data, key.length, element);
    return element;
}

static void cxHashSetUnsafe(cxHash hash,cxHashKey key,cxAny any)
{
    cxHashElement *new = allocator->malloc(sizeof(cxHashElement));
    memcpy(new->key, key.data, key.length);
    new->key[key.length] = '\0';
    new->any = any;
    HASH_ADD(hh, hash->hashPtr, key, key.length, new);
    CX_RETAIN(new->any);
}

cxBool cxHashSet(cxHash hash,cxHashKey key,cxAny any)
{
    cxHashElement *element = cxHashGetElement(hash, key);
    if(element == NULL){
        cxHashSetUnsafe(hash, key, any);
        return false;
    }
    if(element->any != any){
        CX_RETAIN_SWAP(element->any, any);
    }
    return true;
}











