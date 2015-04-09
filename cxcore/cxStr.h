//
//  cxStr.h
//  cxCore
//  可以存放二进制数据
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxStr_h
#define cxCore_cxStr_h

#include "utstring.h"
#include <ctype.h>
#include "cxCore.h"
#include "cxArray.h"
#include "cxHash.h"
#include "utf8.h"

CX_C_BEGIN

typedef enum {
    cxStrEncodeUTF8,
    cxStrEncodeUTF16,
    cxStrEncodeUTF32,
}cxStrEncode;

CX_DEF(cxStr, cxObject)
    cxBool nocopy;
    UT_string strptr;
    CX_FIELD_DEF(cxStrEncode Encode);//default utf8
CX_END(cxStr, cxObject)

CX_FIELD_GET(cxStr, cxStrEncode, Encode);

#define AES_KEY_LENGTH  16

#define cxStrNumber(v,t)         cxStrData(&(t){v},sizeof(t))

#define cxStrEquChars(s1,c1)     (cxStrOK(s1) && cxConstCharsEqu(cxStrBody(s1),c1))

#define cxConstCharsEqu(s1,s2)      ((s1) != NULL && (s2) != NULL && strcmp(s1,s2) == 0)

#define cxConstCharsHas(str,sub)    (str != NULL && sub != NULL && strstr(str,sub) != NULL)

#define cxConstCaseCharsEqu(s1,s2)  ((s1) != NULL && (s2) != NULL && strcasecmp(s1,s2) == 0)

#define cxConstCharsOK(s)           ((s) != NULL && strlen(s) > 0)

#define cxConstCharsLength(s)       ((cxInt)strlen(s))

#define cxStrOK(s)               ((s) != NULL && cxStrLength(s) > 0)

#define cxConstStr(f,...)        cxStrBody(cxStrCreate(f,##__VA_ARGS__))

#define cxConstBinary(d,l)          cxStrBody(cxStrBinary(d,l))

#define UTF8(f,...)                 cxStrCreate(f,##__VA_ARGS__)

cxStr cxStrToUTF8(cxStr str);

cxStr cxStrToUTF16(cxStr str);

cxStr cxStrToUTF32(cxStr str);

cxBool cxStrIsEmpty(cxStr str);

cxStr cxStrEmpty();

cxAny cxStrBody(cxStr string);

cxHashKey cxStrHashKey(cxStr str);

void cxStrConcat(cxStr string,cxStr str);

cxInt cxStrLength(cxStr string);

void cxStrAppend(cxStr string,cxConstChars d,cxInt l);

cxStr cxStrBinary(cxAny d,cxInt l);

void cxStrPrint(cxStr str);

cxBool cxConstCharsIsNumber(cxConstChars s);

cxStr cxStrNoCopy(cxAny d,cxInt l);

cxStr cxStrAttachMem(cxChars d,cxInt l);

cxStr cxStrCreate(cxConstChars format,...);

void cxStrFormatVA(cxStr string,cxConstChars format,va_list ap);

void cxStrFormat(cxStr string,cxConstChars format,...);

void cxStrAppendByte(cxStr str,cxByte b);

cxInt cxRand(cxInt min,cxInt max);

void cxSetRandSeed();

cxBool cxStrToBool(cxStr str,cxBool dv);

cxInt cxStrToInt(cxStr str,cxInt dv);

cxDouble cxStrToDouble(cxStr str,cxDouble dv);

cxLong cxStrToLong(cxStr str,cxLong dv);

cxArray cxStrSplit(cxStr string,cxConstChars sp);

void cxStrClear(cxStr string);

void cxStrErase(cxStr str,cxInt c);

//1 from localized,2 from property
cxInt cxConstCharsTypePath(cxConstChars s,cxChars path);

cxBool cxStrHasConstChars(cxStr str,cxConstChars cs);

cxBool cxStrHasChar(cxStr str,cxChar c);

cxBool cxStrHasChars(cxStr str,cxChars cs);

cxBool cxStrEqu(cxStr s1,cxStr s2);

cxBool cxStrHasPrefix(cxStr str,cxConstChars prefix);

cxBool cxStrHasSuffix(cxStr str,cxConstChars suffix);

cxStr cxStrAttachChars(cxChars str);

cxStr cxStrConstChars(cxConstChars str);

cxInt cxStrUTF8Length(cxStr str);

void cxStrReplace(cxStr string,cxChar find,cxChar rep);

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c);

//user zlib
cxStr cxZCompressed(cxStr data);

cxStr cxZDecompress(cxStr data);

//use aes cbc
cxStr cxAESDecode(cxStr data,cxStr key);

cxStr cxAESEncode(cxStr data,cxStr key);

cxStr cxMD5(cxStr v);
//use xxtea

cxStr cxTEAEncode(cxStr data,cxStr key);

cxStr cxTEADecode(cxStr data,cxStr key);

CX_C_END

#endif
