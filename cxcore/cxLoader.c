//
//  cxLoader.c
//  cxCore
//
//  Created by xuhua on 9/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxLoader.h"

CX_TYPE(cxLoader, cxObject)
{
    
}
CX_INIT(cxLoader, cxObject)
{
    this->objects = CX_ALLOC(cxHash);
}
CX_FREE(cxLoader, cxObject)
{
    CX_RELEASE(this->Root);
    CX_RELEASE(this->objects);
}
CX_TERM(cxLoader, cxObject)

cxAny cxLoaderGet(cxAny loader,cxConstChars id)
{
    CX_ASSERT_THIS(loader, cxLoader);
    return cxHashGet(this->objects, cxHashStrKey(id));
}

cxLoader cxLoaderCreate(cxConstChars path)
{
    cxJson json = cxJsonRead(path);
    CX_ASSERT(json != NULL, "json file %s read error",path);
    cxLoader this = CX_CREATE(cxLoader);
    cxCorePush(cxCoreStackTypeLoader,this);
    CX_RETAIN_SWAP(this->Root, cxObjectCreateUseJson(json));
    cxCorePop(cxCoreStackTypeLoader);
    return this;
}