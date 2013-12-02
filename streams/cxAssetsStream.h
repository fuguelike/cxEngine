//
//  cxAssetsStream.h
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAssetsStream_h
#define cxEngine_cxAssetsStream_h

#include <core/cxBase.h>
#include <core/cxStream.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxAssetsStream, cxStream)
    FILE *fd;
CX_OBJECT_END(cxAssetsStream)

cxStream cxAssetsStreamCreate(cxConstChars file);

cxString cxAssertsData(cxConstChars file);

CX_C_END

#endif
