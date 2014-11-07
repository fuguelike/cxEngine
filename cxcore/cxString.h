//
//  cxString.h
//  cxCore
//  可以存放二进制数据
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxString_h
#define cxCore_cxString_h

#include <utstring.h>
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

#define cxStringOK(s)               ((s) != NULL && cxStringLength(s) > 0)

#define cxConstString(f,...)        cxStringBody(cxStringCreate(f,##__VA_ARGS__))

#define UTF8(f,...)                 cxStringCreate(f,##__VA_ARGS__)

CX_INLINE cxAny cxStringBody(cxString string)
{
    CX_RETURN(string == NULL,NULL);
    return utstring_body(&string->strptr);
}

CX_INLINE void cxStringConcat(cxString string,cxString str)
{
    utstring_concat(&string->strptr, &str->strptr);
}

CX_INLINE cxInt cxStringLength(cxString string)
{
    CX_RETURN(string == NULL, 0);
    return utstring_len(&string->strptr);
}

CX_INLINE void cxStringAppend(cxString string,cxConstChars d,cxInt l)
{
    CX_ASSERT(l > 0 && d != NULL, "args error");
    utstring_bincpy(&string->strptr, d, l);
}

CX_INLINE cxString cxStringBinary(cxAny d,cxInt l)
{
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, d, l);
    return rv;
}

CX_INLINE cxBool cxConstCharsIsNumber(cxConstChars s)
{
    CX_RETURN(s == NULL, false);
    for(cxInt i = 0; i < strlen(s); i++){
        if(!isdigit(s[i])){
            return false;
        }
    }
    return true;
}

CX_INLINE cxString cxStringNoCopy(cxAny d,cxInt l)
{
    cxString rv = CX_CREATE(cxString);
    rv->strptr.d = d;
    rv->strptr.i = l;
    rv->strptr.n = l;
    rv->nocopy = true;
    return rv;
}

CX_INLINE cxString cxStringAttachMem(cxChars d,cxInt l)
{
    cxString rv = CX_CREATE(cxString);
    rv->strptr.d = d;
    rv->strptr.i = l;
    rv->strptr.n = l;
    return rv;
}

CX_INLINE cxString cxStringCreate(cxConstChars format,...)
{
    cxString rv = CX_CREATE(cxString);
    va_list ap;
    va_start(ap, format);
    utstring_printf_va(&rv->strptr, format, ap);
    va_end(ap);
    return rv;
}

CX_INLINE void cxStringFormat(cxString string,cxConstChars format,...)
{
    va_list ap;
    va_start(ap, format);
    utstring_printf_va(&string->strptr, format, ap);
    va_end(ap);
}

CX_INLINE void cxStringAppendByte(cxString str,cxByte b)
{
    utstring_bincpy(&str->strptr, &b, 1);
}

CX_INLINE cxInt cxRand(cxInt min,cxInt max)
{
    cxInt x = rand();
    return (min + x % (max + 1 - min));
}

CX_INLINE void cxSetRandSeed()
{
    srand((unsigned)time(NULL));
}

CX_INLINE cxBool cxStringToBool(cxString str,cxBool dv)
{
    CX_RETURN(!cxStringOK(str), dv);
    return strcmp(str->strptr.d, "true") == 0;
}

CX_INLINE cxInt cxStringToInt(cxString str,cxInt dv)
{
    CX_RETURN(!cxStringOK(str), dv);
    return atoi(str->strptr.d);
}

CX_INLINE cxDouble cxStringToDouble(cxString str,cxDouble dv)
{
    CX_RETURN(!cxStringOK(str), dv);
    return atof(str->strptr.d);
}

CX_INLINE cxLong cxStringToLong(cxString str,cxLong dv)
{
    CX_RETURN(!cxStringOK(str), dv);
    return atol(str->strptr.d);
}

CX_INLINE void cxStringClear(cxString string)
{
    if(cxStringLength(string) > 0){
        utstring_clear(&string->strptr);
    }
}

CX_INLINE void cxStringErase(cxString str,cxInt c)
{
    cxInt l = cxStringLength(str);
    CX_RETURN(c == 0 || l < abs(c));
    cxInt r = l - abs(c);
    if(r == 0){
        cxStringClear(str);
        return;
    }
    if(c > 0){
        memmove(str->strptr.d, str->strptr.d + c, r);
    }
    str->strptr.i = r;
    str->strptr.d[r] = '\0';
}

CX_INLINE cxBool cxStringHasConstChars(cxString str,cxConstChars cs)
{
    CX_ASSERT_THIS(str, cxString);
    if(!cxStringOK(this) || !cxConstCharsOK(cs)){
        return false;
    }
    cxConstChars body = cxStringBody(this);
    return cxConstCharsHas(body, cs);
}

CX_INLINE cxBool cxStringEqu(cxString s1,cxString s2)
{
    if(s1 == NULL && s2 == NULL){
        return true;
    }
    if(s1 == NULL || s2 == NULL){
        return false;
    }
    if(s1->strptr.i != s2->strptr.i){
        return false;
    }
    return memcmp(s1->strptr.d, s2->strptr.d,s1->strptr.i) == 0;
}

CX_INLINE cxString cxStringAttachChars(cxChars str)
{
    CX_ASSERT(str != NULL, "str null");
    return cxStringAttachMem(str, (cxInt)strlen(str));
}

CX_INLINE cxString cxStringConstChars(cxConstChars str)
{
    CX_ASSERT(str != NULL, "str null");
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, (void *)str, (cxInt)strlen(str));
    return rv;
}

CX_INLINE void cxStringReplace(cxString string,cxChar find,cxChar rep)
{
    cxChars ptr = (cxChars)cxStringBody(string);
    cxInt len = cxStringLength(string);
    for(cxInt i=0; i < len; i++){
        if(ptr[i] == find){
            ptr[i] = rep;
        }
    }
}

CX_INLINE cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c)
{
    for(cxInt i=0; i < strlen(sp);i++){
        if(sp[i] == c){
            return true;
        }
    }
    return  false;
}

//user zlib
cxString cxZCompressed(cxString data);

cxString cxZDecompress(cxString data);

//use aes cbc
cxString cxAESDecode(cxString data,cxString key);

cxString cxAESEncode(cxString data,cxString key);

cxString cxMD5(cxString v);


CX_C_END

#endif
