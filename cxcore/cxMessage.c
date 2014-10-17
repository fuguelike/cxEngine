//
//  cxMessage.c
//  cxCore
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxMessage.h"

static cxMessage instance = NULL;

CX_DEF(cxMessageItem, cxObject)
    cxMessageFunc func;
    cxAny dst;
CX_END(cxMessageItem, cxObject)

CX_TYPE(cxMessageItem, cxObject)
{}
CX_INIT(cxMessageItem, cxObject)
{}
CX_FREE(cxMessageItem, cxObject)
{}
CX_TERM(cxMessageItem, cxObject)

void cxMessageDestroy()
{
    CX_RELEASE(instance);
    instance = NULL;
}

cxMessage cxMessageInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxMessage);
    }
    return instance;
}

CX_TYPE(cxMessage, cxObject)
{}
CX_INIT(cxMessage, cxObject)
{
    this->keys = CX_ALLOC(cxHash);
}
CX_FREE(cxMessage, cxObject)
{
    CX_RELEASE(this->keys);
}
CX_TERM(cxMessage, cxObject)

void cxMessageRemove(cxAny dst)
{
    cxMessage this = cxMessageInstance();
    CX_HASH_FOREACH(this->keys, keye, keyt){
        cxHash list = keye->any;
        CX_HASH_FOREACH(list, vale, valt){
            cxMessageItem item = vale->any;
            if(item->dst != dst){
                continue;
            }
            cxHashDelElement(list, vale);
        }
        if(cxHashLength(list) == 0){
            cxHashDelElement(this->keys, keye);
        }
    }
}

void cxMessageRemoveKey(cxAny dst,cxConstChars key)
{
    cxMessage this = cxMessageInstance();
    cxHash list = cxHashGet(this->keys, cxHashAnyKey(key));
    CX_RETURN(list == NULL || cxHashLength(list) == 0);
    cxHashDel(list, cxHashAnyKey(dst));
}

void cxMessagePost(cxConstChars key,cxAny src)
{
    cxMessage this = cxMessageInstance();
    cxHash list = cxHashGet(this->keys, cxHashAnyKey(key));
    CX_RETURN(list == NULL || cxHashLength(list) == 0);
    CX_HASH_FOREACH(list, ele, tmp){
        cxMessageItem item = ele->any;
        item->func(item->dst,src);
    }
}

void cxMessageAppend(cxAny dst,cxAny func,cxConstChars key)
{
    cxHashKey skey = cxHashAnyKey(key);
    cxMessage this = cxMessageInstance();
    cxHash list = cxHashGet(this->keys, skey);
    if(list == NULL){
        list = CX_ALLOC(cxHash);
        cxHashSet(this->keys, skey, list);
        CX_RELEASE(list);
    }
    cxMessageItem item = CX_ALLOC(cxMessageItem);
    item->dst = dst;
    item->func = func;
    cxHashSet(list, cxHashAnyKey(dst), item);
    CX_RELEASE(item);
}









