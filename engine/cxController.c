//
//  cxController.c
//  cxEngine
//
//  Created by xuhua on 5/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxView.h"
#include "cxController.h"

CX_OBJECT_TYPE(cxController, cxObject)
{
    
}
CX_OBJECT_INIT(cxController, cxObject)
{
    this->objects = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxController, cxObject)
{
    CX_RELEASE(this->objects);
    CX_RELEASE(this->root);
}
CX_OBJECT_TERM(cxController, cxObject)

cxAny cxControllerObject(cxAny controller,cxConstChars id)
{
    cxController this = controller;
    return cxHashGet(this->objects, cxHashStrKey(id));
}

void cxControllerInitWithFile(cxAny controller, cxConstChars file)
{
    cxEngine engine = cxEngineInstance();
    cxController this = controller;
    cxStackPush(engine->stack, this);
    cxAny root = cxObjectLoadWithFile(file);
    cxStackPop(engine->stack);
    CX_ASSERT(root != NULL, "load root view %s failed",file);
    CX_RETAIN_SWAP(this->root, root);
}

cxController cxControllerCreate(cxConstChars file)
{
    cxController this = CX_CREATE(cxController);
    cxControllerInitWithFile(this, file);
    return this;
}

