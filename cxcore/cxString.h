//
//  cxString.h
//  cxCore
//  可以存放二进制数据
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxString_h
#define cxCore_cxString_h

#include "utstring.h"
#include <ctype.h>
#include "cxCore.h"
#include "cxArray.h"

CX_C_BEGIN

CX_DEF(cxString, cxObject)
    cxBool nocopy;
    UT_string strptr;
CX_END(cxString, cxObject)

#define AES_KEY_LENGTH  16

#define cxStringNumber(v,t)         cxStringData(&(t){v},sizeof(t))

#define cxConstCharsEqu(s1,s2)      ((s1) != NULL && (s2) != NULL && strcmp(s1,s2) == 0)

#define cxConstCharsHas(str,sub)    (str != NULL && sub != NULL && strstr(str,sub) != NULL)

#define cxConstCaseCharsEqu(s1,s2)  ((s1) != NULL && (s2) != NULL && strcasecmp(s1,s2) == 0)

#define cxConstCharsOK(s)           ((s) != NULL && strlen(s) > 0)

#define cxConstCharsLength(s)       ((cxInt)strlen(s))

#define cxStringOK(s)               ((s) != NULL && cxStringLength(s) > 0)

#define cxConstString(f,...)        cxStringBody(cxStringCreate(f,##__VA_ARGS__))

#define cxConstBinary(d,l)          cxStringBody(cxStringBinary(d,l))

#define UTF8(f,...)                 cxStringCreate(f,##__VA_ARGS__)

cxAny cxStringBody(cxString string);

void cxStringConcat(cxString string,cxString str);

cxInt cxStringLength(cxString string);

void cxStringAppend(cxString string,cxConstChars d,cxInt l);

cxString cxStringBinary(cxAny d,cxInt l);

cxBool cxConstCharsIsNumber(cxConstChars s);

cxString cxStringNoCopy(cxAny d,cxInt l);

cxString cxStringAttachMem(cxChars d,cxInt l);

cxString cxStringCreate(cxConstChars format,...);

void cxStringFormat(cxString string,cxConstChars format,...);

void cxStringAppendByte(cxString str,cxByte b);

cxInt cxRand(cxInt min,cxInt max);

void cxSetRandSeed();

cxBool cxStringToBool(cxString str,cxBool dv);

cxInt cxStringToInt(cxString str,cxInt dv);

cxDouble cxStringToDouble(cxString str,cxDouble dv);

cxLong cxStringToLong(cxString str,cxLong dv);

void cxStringClear(cxString string);

void cxStringErase(cxString str,cxInt c);

//1 from localized,2 from property
cxInt cxConstCharsTypePath(cxConstChars s,cxChars path);

cxBool cxStringHasConstChars(cxString str,cxConstChars cs);

cxBool cxStringEqu(cxString s1,cxString s2);

cxString cxStringAttachChars(cxChars str);

cxString cxStringConstChars(cxConstChars str);

void cxStringReplace(cxString string,cxChar find,cxChar rep);

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c);

//user zlib
cxString cxZCompressed(cxString data);

cxString cxZDecompress(cxString data);

//use aes cbc
cxString cxAESDecode(cxString data,cxString key);

cxString cxAESEncode(cxString data,cxString key);

cxString cxMD5(cxString v);


CX_C_END

#endif
