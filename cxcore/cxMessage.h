//
//  cxMessage.h
//  cxCore
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxMessage_h
#define cxCore_cxMessage_h

#include "cxBase.h"

CX_C_BEGIN

typedef void (*cxMessageFunc)(cxAny dst,cxAny src);

CX_OBJECT_DEF(cxMessage, cxObject)
    cxHash keys;
CX_OBJECT_END(cxMessage)

void cxMessageDestroy();

void cxMessageRemove(cxAny dst);

void cxMessageRemoveKey(cxAny dst,cxInt key);

void cxMessagePost(cxInt key,cxAny src);

void cxMessageAppend(cxAny dst,cxAny func,cxInt key);

CX_C_END

#endif
