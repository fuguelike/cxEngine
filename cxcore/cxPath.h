//
//  cxPath.h
//  cxEngine
//
//  Created by xuhua on 12/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxPath_h
#define cxCore_cxPath_h

#include "cxCore.h"
#include "cxHash.h"

CX_C_BEGIN

#define CX_PATH_DEF(_n_,_f_,...)    cxChar _n_[PATH_MAX];_n_[snprintf(_n_, PATH_MAX, _f_,__VA_ARGS__)]='\0';

//path?key=value
CX_DEF(cxPath, cxObject)
    cxChar path[CX_HASH_MAX_KEY];
    cxChar key[CX_HASH_MAX_KEY];
    cxChar value[CX_HASH_MAX_KEY];
    cxInt count;
CX_END(cxPath, cxObject)

cxPath cxPathParseKeyValue(cxPath this);

cxPath cxPathParse(cxConstChars url);

CX_C_END

#endif
