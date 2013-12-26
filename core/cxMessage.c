//
//  cxMessage.c
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEventArg.h"
#include "cxMessage.h"

static cxMessage instance = NULL;

CX_OBJECT_DEF(cxMessageItem, cxObject)
    cxMessageFunc func;
    cxAny dst;
CX_OBJECT_END(cxMessageItem)

CX_OBJECT_INIT(cxMessageItem, cxObject)
{
}
CX_OBJECT_FREE(cxMessageItem, cxObject)
{
}
CX_OBJECT_TERM(cxMessageItem, cxObject)

void cxMessageDestroy()
{
    CX_RELEASE(instance);
    instance = NULL;
}

static cxMessage cxMessageInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxMessage);
    }
    return instance;
}

CX_OBJECT_INIT(cxMessage, cxObject)
{
    this->keys = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxMessage, cxObject)
{
    CX_RELEASE(this->keys);
}
CX_OBJECT_TERM(cxMessage, cxObject)

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
    cxHash list = cxHashGet(this->keys, cxHashStrKey(key));
    CX_RETURN(list == NULL || cxHashLength(list) == 0);
    cxHashDel(list, cxHashIntKey((cxInt)dst));
}

void cxMessagePost(cxConstChars key,cxAny src)
{
    cxMessage this = cxMessageInstance();
    cxHash list = cxHashGet(this->keys, cxHashStrKey(key));
    CX_RETURN(list == NULL || cxHashLength(list) == 0);
    CX_HASH_FOREACH(list, ele, tmp){
        cxMessageItem item = ele->any;
        item->func(item->dst,src);
    }
}

void cxMessageAppend(cxAny dst,cxAny func,cxConstChars key)
{
    cxMessage this = cxMessageInstance();
    cxHash list = cxHashGet(this->keys, cxHashStrKey(key));
    if(list == NULL){
        list = CX_ALLOC(cxHash);
        cxHashSet(this->keys, cxHashStrKey(key), list);
        CX_RELEASE(list);
    }
    cxMessageItem item = CX_ALLOC(cxMessageItem);
    item->dst = dst;
    item->func = func;
    cxHashSet(list, cxHashIntKey((cxInt)dst), item);
    CX_RELEASE(item);
}









