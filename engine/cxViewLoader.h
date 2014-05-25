//
//  cxViewLoader.h
//  cxEngine
//
//  Created by xuhua on 5/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxViewLoader_h
#define cxEngine_cxViewLoader_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxViewLoader, cxObject)
    cxAny view;
    cxHash objects;
CX_OBJECT_END(cxViewLoader)

cxAny cxViewLoaderObject(cxAny controller,cxConstChars id);

void cxViewLoaderInitWithFile(cxAny controller, cxConstChars file);

cxViewLoader cxViewLoaderCreate(cxConstChars file);

CX_C_END

#endif
