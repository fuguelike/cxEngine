//
//  cxHash.c
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxType.h"
#include "cxHash.h"

CX_OBJECT_TYPE(cxHash, cxObject)
{}
CX_OBJECT_INIT(cxHash, cxObject)
{}
CX_OBJECT_FREE(cxHash, cxObject)
{
    cxHashClear(this);
}
CX_OBJECT_TERM(cxHash, cxObject)

#define CX_HASH_KEY_OK(_k_) ((_k_).data != NULL && (_k_).length > 0 && (_k_).length < CX_HASH_MAX_KEY)

void cxHashClear(cxAny phash)
{
    CX_ASSERT_THIS(phash, cxHash);
    cxHashElement *ele = NULL, *tmp = NULL;
    HASH_ITER(hh, this->hashPtr, ele, tmp){
        CX_RELEASE(ele->any);
    }
    HASH_CLEAR(hh, this->hashPtr);
}

void cxHashDelElement(cxAny phash,cxHashElement *element)
{
    CX_ASSERT_THIS(phash, cxHash);
    HASH_DEL(this->hashPtr, element);
    CX_RELEASE(element->any);
    allocator->free(element);
}

cxBool cxHashHas(cxAny phash,cxHashKey key)
{
    CX_ASSERT_THIS(phash, cxHash);
    CX_ASSERT(CX_HASH_KEY_OK(key), "hash key error");
    cxHashElement *element = NULL;
    HASH_FIND(hh, this->hashPtr, key.data, key.length, element);
    return element != NULL;
}

cxBool cxHashDel(cxAny phash,cxHashKey key)
{
    CX_ASSERT_THIS(phash, cxHash);
    cxHashElement *element = NULL;
    HASH_FIND(hh, this->hashPtr, key.data, key.length, element);
    if(element != NULL){
        cxHashDelElement(this, element);
        return true;
    }
    return false;
}

cxInt cxHashLength(cxAny phash)
{
    CX_ASSERT_THIS(phash, cxHash);
    return HASH_COUNT(this->hashPtr);
}

cxAny cxHashFirst(cxAny phash)
{
    CX_ASSERT_THIS(phash, cxHash);
    cxHashElement *ele = NULL, *tmp = NULL;
    HASH_ITER(hh, this->hashPtr, ele, tmp){
        return ele->any;
    }
    return NULL;
}

cxAny cxHashGet(cxAny phash,cxHashKey key)
{
    CX_ASSERT_THIS(phash, cxHash);
    cxHashElement *element = cxHashGetElement(this, key);
    return element == NULL ? NULL : element->any;
}

cxHashElement *cxHashGetElement(cxAny phash,cxHashKey key)
{
    CX_ASSERT_THIS(phash, cxHash);
    CX_ASSERT(CX_HASH_KEY_OK(key), "hash key error");
    cxHashElement *element = NULL;
    HASH_FIND(hh, this->hashPtr, key.data, key.length, element);
    return element;
}

static void cxHashSetUnsafe(cxAny phash,cxHashKey key,cxAny any)
{
    CX_ASSERT_THIS(phash, cxHash);
    cxHashElement *new = allocator->malloc(sizeof(cxHashElement));
    memcpy(new->key, key.data, key.length);
    new->any = any;
    HASH_ADD(hh, this->hashPtr, key, key.length, new);
    CX_RETAIN(new->any);
}

cxBool cxHashSet(cxAny phash,cxHashKey key,cxAny any)
{
    CX_ASSERT_THIS(phash, cxHash);
    cxHashElement *element = cxHashGetElement(this, key);
    if(element == NULL){
        cxHashSetUnsafe(this, key, any);
        return false;
    }
    if(element->any != any){
        CX_RETAIN_SWAP(element->any, any);
    }
    return true;
}











