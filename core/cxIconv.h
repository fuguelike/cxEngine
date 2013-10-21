//
//  cxIconv.h
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxIconv_h
#define cxEngine_cxIconv_h

#include <iconv.h>
#include "cxBase.h"
#include "cxString.h"
#include "cxHash.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxIconvItem, cxObject)
    iconv_t iconvptr;
    cxString from;
    cxString to;
CX_OBJECT_END(cxIconvItem)

CX_OBJECT_DEF(cxIconv, cxObject)
    cxHash caches;
CX_OBJECT_END(cxIconv)

cxIconv cxIconvInstance();

cxString cxIconvConvert(const cxString string,cxConstChars from,cxConstChars to);

void cxIconvDestroy();

CX_C_END

#endif
