//
//  cxController.c
//  cxEngine
//
//  Created by xuhua on 5/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxView.h"
#include "cxController.h"

CX_OBJECT_INIT(cxController, cxObject)
{
    
}
CX_OBJECT_FREE(cxController, cxObject)
{
    CX_RELEASE(this->views);
    CX_RELEASE(this->rootView);
}
CX_OBJECT_TERM(cxController, cxObject)

cxAny cxControllerFindView(cxAny pc,cxConstChars key)
{
    cxController this = pc;
    return cxHashGet(this->views, cxHashStrKey(key));
}

void cxControllerLoadView(cxAny pc, cxConstChars file)
{
    cxController this = pc;
    cxAny rootView = cxViewLoad(file, this->views);
    CX_ASSERT(rootView != NULL, "load root view %s failed",file);
    CX_RETAIN_SWAP(this->rootView, rootView);
}

