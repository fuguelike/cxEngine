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

CX_OBJECT_INIT(cxMsgItem, cxObject)
{
    
}
CX_OBJECT_FREE(cxMsgItem, cxObject)
{
    
}
CX_OBJECT_TERM(cxMsgItem, cxObject)

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

void cxMessagePostEvent(cxEvent *event)
{
    CX_RETURN(event->args == NULL);
    cxConstChars skey = cxEventArgToString(event->args);
    CX_RETURN(skey == NULL);
    cxMessagePost(skey, event->object);
}

void cxMessageRemove(cxAny dst)
{
    cxMessage this = cxMessageInstance();
    CX_HASH_FOREACH(this->keys, keye, keyt){
        cxHash list = keye->any;
        CX_HASH_FOREACH(list, vale, valt){
            cxMsgItem item = vale->any;
            if(item->dst != dst){
                continue;
            }
            cxHashDelElement(list, vale);
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
        cxMsgItem item = ele->any;
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
    cxMsgItem item = CX_ALLOC(cxMsgItem);
    item->dst = dst;
    item->func = func;
    cxHashSet(list, cxHashIntKey((cxInt)dst), item);
    CX_RELEASE(item);
}









