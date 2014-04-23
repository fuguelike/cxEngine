//
//  cxString.h
//  cxEngine
//  可以存放二进制数据
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxString_h
#define cxEngine_cxString_h

#include "utstring.h"
#include "cxFramework.h"
#include "cxMD5.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxString, cxObject)
    UT_string strptr;
CX_OBJECT_END(cxString)

void cxMD5(cxString v,cxChar md5[MD5_DIGEST_LENGTH*2 + 1]);

void cxStringClean(cxString string);

cxConstChars cxStringBody(cxString string);

void cxStringConcat(cxString string,cxString str);

cxInt cxStringLength(cxString string);

void cxStringFormat(cxString string,cxConstChars format,...);

void cxStringAppend(cxString string,cxConstChars d,cxInt l);

CX_C_END

#endif
