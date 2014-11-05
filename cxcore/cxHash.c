//
//  cxHash.c
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxType.h"
#include "cxHash.h"

/*
 items:{
    "key1":value1,
    "key2":value2,
 }
 */

CX_SETTER_DEF(cxHash, items)
{
    CX_ASSERT(cxJsonIsObject(value), "items must is object");
    cxJson items = cxJsonToObject(value);
    CX_JSON_OBJECT_EACH_BEG(items, v)
    cxHashKey key = cxHashStrKey(vKey);
    cxAny any = cxJsonTocxObject(v);
    if(any != NULL){
        cxHashSet(this, key, any);
    }
    CX_JSON_OBJECT_EACH_END(items, v)
}

CX_TYPE(cxHash, cxObject)
{
    CX_SETTER(cxHash, items);
}
CX_INIT(cxHash, cxObject)
{
    
}
CX_FREE(cxHash, cxObject)
{
    cxHashClear(this);
}
CX_TERM(cxHash, cxObject)

#define CX_HASH_KEY_OK(_k_) ((_k_).data != NULL && (_k_).length > 0 && (_k_).length < CX_HASH_MAX_KEY)

void cxHashClear(cxAny phash)
{
    CX_ASSERT_THIS(phash, cxHash);
    cxHashElement *ele = NULL, *tmp = NULL;
    HASH_ITER(hh, this->hptr, ele, tmp){
        CX_RELEASE(ele->any);
    }
    HASH_CLEAR(hh, this->hptr);
}

void cxHashDelElement(cxAny phash,cxHashElement *element)
{
    CX_ASSERT_THIS(phash, cxHash);
    HASH_DEL(this->hptr, element);
    CX_RELEASE(element->any);
    allocator->free(element);
}

cxBool cxHashHas(cxAny phash,cxHashKey key)
{
    CX_ASSERT_THIS(phash, cxHash);
    CX_ASSERT(CX_HASH_KEY_OK(key), "hash key error");
    cxHashElement *element = NULL;
    HASH_FIND(hh, this->hptr, key.data, key.length, element);
    return element != NULL;
}

cxBool cxHashDel(cxAny phash,cxHashKey key)
{
    CX_ASSERT_THIS(phash, cxHash);
    cxHashElement *element = NULL;
    HASH_FIND(hh, this->hptr, key.data, key.length, element);
    if(element != NULL){
        cxHashDelElement(this, element);
        return true;
    }
    return false;
}

cxInt cxHashLength(cxAny phash)
{
    CX_ASSERT_THIS(phash, cxHash);
    return HASH_COUNT(this->hptr);
}

cxAny cxHashFirst(cxAny phash)
{
    CX_ASSERT_THIS(phash, cxHash);
    return (this->hptr == NULL) ? NULL : this->hptr->any;
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
    HASH_FIND(hh, this->hptr, key.data, key.length, element);
    return element;
}

static void cxHashSetUnsafe(cxAny phash,cxHashKey key,cxAny any)
{
    CX_ASSERT_THIS(phash, cxHash);
    cxHashElement *new = allocator->malloc(sizeof(cxHashElement));
    memcpy(new->key, key.data, key.length);
    new->any = any;
    HASH_ADD(hh, this->hptr, key, key.length, new);
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











