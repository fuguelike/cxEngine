//
//  cxMessage.c
//  cxCore
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxMessage.h"

static cxMessage instance = NULL;

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

void cxMessageClear()
{
    CX_ASSERT(instance != NULL, "message not init");
    cxHashClear(instance->keys);
}

cxMessage cxMessageInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxMessage);
    }
    return instance;
}

CX_TYPE(cxMessage, cxObject)
{
    
}
CX_INIT(cxMessage, cxObject)
{
    this->keys = CX_ALLOC(cxHash);
}
CX_FREE(cxMessage, cxObject)
{
    CX_RELEASE(this->keys);
}
CX_TERM(cxMessage, cxObject)

void cxMessageRemoveKey(cxAny dst,cxConstChars key)
{
    cxMessage this = cxMessageInstance();
    cxHash list = cxHashGet(this->keys, cxHashStrKey(key));
    CX_RETURN(list == NULL);
    cxHashDel(list, cxHashAnyKey(dst));
}

void cxMessagePost(cxConstChars key,cxAny info)
{
    cxMessage this = cxMessageInstance();
    cxHash list = cxHashGet(this->keys, cxHashStrKey(key));
    CX_RETURN(list == NULL);
    CX_HASH_FOREACH(list, ele, tmp){
        cxMessageItem item = ele->any;
        cxMessageFunc func = (cxMessageFunc)item->func;
        if(func(item->dst,info)){
            break;
        }
    }
}

cxMessageItem cxMessageAppend(cxAny dst,cxMessageFunc func,cxConstChars key)
{
    cxObjectSetAttr(dst, cxObjectAttrMessage);
    cxHashKey skey = cxHashStrKey(key);
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
    return item;
}









