//
//  cxViewLoader.c
//  cxEngine
//
//  Created by xuhua on 5/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxView.h"
#include "cxViewLoader.h"

CX_OBJECT_TYPE(cxViewLoader, cxObject)
{
    
}
CX_OBJECT_INIT(cxViewLoader, cxObject)
{
    this->objects = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxViewLoader, cxObject)
{
    CX_RELEASE(this->objects);
    CX_RELEASE(this->view);
}
CX_OBJECT_TERM(cxViewLoader, cxObject)

cxAny cxViewLoaderObject(cxAny controller,cxConstChars id)
{
    cxViewLoader this = controller;
    return cxHashGet(this->objects, cxHashStrKey(id));
}

void cxViewLoaderInitWithFile(cxAny controller, cxConstChars file)
{
    cxViewLoader this = controller;
    cxEnginePush(this);
    cxAny view = cxObjectLoadWithFile(file);
    cxEnginePop();
    CX_ASSERT(view != NULL, "load root view %s failed",file);
    CX_RETAIN_SWAP(this->view, view);
}

cxViewLoader cxViewLoaderCreate(cxConstChars file)
{
    cxViewLoader this = CX_CREATE(cxViewLoader);
    cxViewLoaderInitWithFile(this, file);
    return this;
}

