//
//  cxController.h
//  cxEngine
//
//  Created by xuhua on 5/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxController_h
#define cxEngine_cxController_h

#include "cxBase.h"
#include "cxHash.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxController, cxObject)
    cxAny rootView;
    cxHash views;
CX_OBJECT_END(cxController)

void cxControllerLoadView(cxAny pc, cxConstChars file);

cxAny cxControllerFindView(cxAny pc,cxConstChars key);

CX_C_END

#endif
