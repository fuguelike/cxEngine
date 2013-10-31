//
//  cxHttpGet.h
//  cxEngine
//
//  Created by xuhua on 10/31/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxHttpGet_h
#define cxEngine_cxHttpGet_h

#include "cxHttp.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxHttpGet, cxHttp)

CX_OBJECT_END(cxHttpGet)

cxHttp cxHttpGetCreate(cxConstChars url,cxBool chunked);

CX_C_END

#endif
