//
//  cxController.c
//  cxEngine
//
//  Created by xuhua on 5/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxController.h"
#include "cxType.h"

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

cxAny cxControllerGet(cxAny pc,cxConstChars key)
{
    cxController this = pc;
    return cxHashGet(this->objects, cxHashStrKey(key));
}

void cxControllerInitWithFile(cxAny pc, cxConstChars file)
{
    cxController this = pc;
    cxAny root = cxObjectLoad(file, this->objects);
    CX_ASSERT(root != NULL, "load root view %s failed",file);
    CX_RETAIN_SWAP(this->root, root);
}

