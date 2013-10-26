//
//  cxMessage.h
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxMessage_h
#define cxEngine_cxMessage_h

#include "cxBase.h"
#include "cxHash.h"

CX_C_BEGIN

typedef void (*cxMessageFunc)(cxAny dst,cxAny src);

CX_OBJECT_DEF(cxMsgItem, cxObject)
    cxMessageFunc func;
    cxAny dst;
CX_OBJECT_END(cxMsgItem)

CX_OBJECT_DEF(cxMessage, cxObject)
    cxHash keys;
CX_OBJECT_END(cxMessage)

void cxMessageDestroy();

void cxMessagePostEvent(cxAny any,cxAny arg);

void cxMessageRemove(cxAny dst);

void cxMessageRemoveKey(cxAny dst,cxConstChars key);

void cxMessagePost(cxConstChars key,cxAny src);

void cxMessageAppend(cxAny dst,cxAny func,cxConstChars key);

CX_C_END

#endif
