//
//  cxMessage.h
//  cxCore
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxMessage_h
#define cxCore_cxMessage_h

#include "cxCore.h"
#include "cxHash.h"

CX_C_BEGIN

#define CX_STRING_KEY_DEF(_k_) static cxConstChars _k_=#_k_

typedef void (*cxMessageFunc)(cxAny dst,cxAny src);

CX_DEF(cxMessage, cxObject)
    cxHash keys;
CX_END(cxMessage, cxObject)

cxMessage cxMessageInstance();

void cxMessageDestroy();

void cxMessageRemove(cxAny dst);

void cxMessageRemoveKey(cxAny dst,cxConstChars key);

void cxMessagePost(cxConstChars key,cxAny src);

void cxMessageAppend(cxAny dst,cxAny func,cxConstChars key);

CX_C_END

#endif
