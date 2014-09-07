//
//  cxLoader.h
//  cxCore
//
//  Created by xuhua on 9/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxLoader_h
#define cxCore_cxLoader_h

#include "cxHash.h"
#include "cxString.h"
#include "cxType.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxLoader, cxObject)
    cxAny object;
    cxHash objects;
CX_OBJECT_END(cxLoader, cxObject)

cxAny cxLoaderGet(cxAny loader,cxConstChars id);

cxLoader cxLoaderCreate(cxConstChars path);

CX_C_END

#endif
