//
//  cxLoader.c
//  cxCore
//
//  Created by xuhua on 9/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxLoader.h"

CX_OBJECT_TYPE(cxLoader, cxObject)
{
    
}
CX_OBJECT_INIT(cxLoader, cxObject)
{
    this->objects = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxLoader, cxObject)
{
    CX_RELEASE(this->object);
    CX_RELEASE(this->objects);
}
CX_OBJECT_TERM(cxLoader, cxObject)

cxAny cxLoaderEnd(cxAny object,cxConstChars id)
{
    cxLoader curr = cxCoreTop();
    if(id != NULL && curr != NULL){
        cxHashSet(curr->objects, cxHashStrKey(id), object);
    }
    return object;
}

cxAny cxLoaderGet(cxAny loader,cxConstChars id)
{
    cxLoader this = loader;
    cxAny object = cxHashGet(this->objects, cxHashStrKey(id));
    CX_ASSERT(object != NULL, "%s object get error",id);
    return object;
}

cxLoader cxLoaderCreate(cxConstChars path)
{
    cxJson json = cxJsonReader(path);
    CX_ASSERT(json != NULL, "json file %s read error",path);
    cxLoader this = CX_CREATE(cxLoader);
    cxCorePush(this);
    cxAny object = cxObjectCreateWithJson(json);
    cxCorePop();
    CX_RETAIN_SWAP(this->object, object);
    return this;
}