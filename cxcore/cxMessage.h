//
//  cxMessage.h
//  cxCore
//  cxMessageFilter(cxAny dst,cxMessageItem item,cxAny info) 如果目标对象有这个方法将会被执行，返回false不会接收消息
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxMessage_h
#define cxCore_cxMessage_h

#include "cxCore.h"
#include "cxHash.h"

CX_C_BEGIN

#define CX_STRING_KEY_DEF(_k_) static cxConstChars _k_=#_k_

CX_DEF(cxMessageItem, cxObject)
    CX_FIELD_DEF(cxLong Tag);
    cxAny func;
    cxAny dst;
CX_END(cxMessageItem, cxObject)

CX_FIELD_IMP(cxMessageItem, cxLong, Tag);

CX_DEF(cxMessage, cxObject)
    cxHash keys;
CX_END(cxMessage, cxObject)

cxMessage cxMessageInstance();

void cxMessageDestroy();

void cxMessageRemove(cxAny dst);

void cxMessageRemoveKey(cxAny dst,cxConstChars key);

void cxMessagePost(cxConstChars key,cxAny info);

cxMessageItem cxMessageAppend(cxAny dst,cxAny func,cxConstChars key);

CX_C_END

#endif
