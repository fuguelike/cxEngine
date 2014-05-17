//
//  cxController.h
//  cxEngine
//
//  Created by xuhua on 5/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxController_h
#define cxEngine_cxController_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxController, cxObject)
    cxAny root;
    cxHash objects;
CX_OBJECT_END(cxController)

cxAny cxControllerObject(cxAny controller,cxConstChars id);

void cxControllerInitWithFile(cxAny controller, cxConstChars file);

cxController cxControllerCreate(cxConstChars file);

CX_C_END

#endif
