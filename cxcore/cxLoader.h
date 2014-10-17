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

CX_DEF(cxLoader, cxObject)
    CX_FIELD_DEF(cxAny Root);
    cxHash objects;
CX_END(cxLoader, cxObject)

cxAny cxLoaderGet(cxAny loader,cxConstChars id);

#define CX_LOADER_DEF(_o_,_t_,_n_)  _t_ _n_ = cxLoaderGet(_o_,#_n_);CX_ASSERT_TYPE(_n_,_t_)

cxLoader cxLoaderCreate(cxConstChars path);

CX_FIELD_GET(cxLoader, cxAny, Root);

CX_C_END

#endif
