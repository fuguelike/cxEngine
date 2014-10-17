//
//  cxAssetsStream.h
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAssetsStream_h
#define cxEngine_cxAssetsStream_h

#include <cxCore/cxBase.h>

CX_C_BEGIN

CX_DEF(cxAssetsStream, cxStream)
    cxAny asset;
CX_END(cxAssetsStream, cxStream)

cxStream cxAssetsStreamCreate(cxConstChars file);

cxString cxAssetsData(cxConstChars file);

CX_C_END

#endif
