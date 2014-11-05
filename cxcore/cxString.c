//
//  cxString.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <ctype.h>
#include <zlib.h>
#include <algorithm/aes.h>
#include "cxMD5.h"
#include "cxString.h"
#include "cxType.h"


CX_SETTER_DEF(cxString, string)
{
    cxConstChars str = cxJsonToConstChars(value);
    if(str == NULL){
        return;
    }
    cxStringFormat(this, "%s",str);
}
CX_SETTER_DEF(cxString, number)
{
    if(cxJsonIsInt(value)){
        cxStringFormat(this, "%d",cxJsonToInt(value, 0));
    }else if(cxJsonIsDouble(value)){
        cxStringFormat(this, "%f",cxJsonToDouble(value, 0));
    }else if(cxJsonIsBool(value)){
        cxStringFormat(this, "%s", cxJsonToBool(value, false) ? "true" : "false");
    }
}

CX_TYPE(cxString, cxObject)
{
    CX_SETTER(cxString, string);
    CX_SETTER(cxString, number);
}
CX_INIT(cxString, cxObject)
{

}
CX_FREE(cxString, cxObject)
{
    if(!this->nocopy){
        utstring_done(&this->strptr);
    }
}
CX_TERM(cxString, cxObject)

cxString cxCompressed(cxString data)
{
    CX_ASSERT(data != NULL, "args error");
    cxInt datasiz = cxStringLength(data);
    uLongf size = compressBound(datasiz);
    char *cv = allocator->malloc(size);
    if(compress((Bytef *)cv, &size, (Bytef *)cxStringBody(data), datasiz) == Z_OK){
        return cxStringAttachMem(cv, (cxInt)size);
    }
    allocator->free(cv);
    return NULL;
}

cxString cxDecompress(cxString data)
{
    CX_ASSERT(data != NULL, "args error");
    cxInt datasiz = cxStringLength(data);
    uLongf size = datasiz * 4;
    char *cv = allocator->malloc(size);
    int ret = uncompress((Bytef *)cv, &size, (Bytef *)cxStringBody(data), (uLongf)datasiz);
    while(ret == Z_BUF_ERROR){
        size = (size << 1);
        cv = allocator->realloc(cv,size);
        ret = uncompress((Bytef *)cv, &size, (Bytef *)cxStringBody(data), (uLongf)datasiz);
    }
    if(ret == Z_OK){
        return cxStringAttachMem(cv, (cxInt)size);
    }
    allocator->free(cv);
    return NULL;
}

cxInt cxRand(cxInt min,cxInt max)
{
    cxInt x = rand();
    return (min + x % (max + 1 - min));
}

void cxSetRandSeed()
{
    srand((unsigned)time(NULL));
}

cxString cxAESDecode(cxString data,cxString key)
{
    CX_ASSERT(cxStringOK(data) && cxStringOK(key), "args error");
    cxUChar iv[AES_KEY_LENGTH]={0};
    cxUChar ik[AES_KEY_LENGTH]={0};
    if(cxStringLength(key) > AES_KEY_LENGTH){
        memcpy(ik, cxStringBody(key), AES_KEY_LENGTH);
    }else {
        memcpy(ik, cxStringBody(key), cxStringLength(key));
    }
    AES_KEY dKey;
    if(AES_set_decrypt_key((const cxUInt8 *)ik, AES_KEY_LENGTH*8, &dKey) != 0){
        return NULL;
    }
    //
    cxConstChars dataptr = cxStringBody(data);
    cxInt length = cxStringLength(data);
    //get iv
    memcpy(iv, dataptr, AES_KEY_LENGTH);
    //
    cxInt bytessize = length - AES_KEY_LENGTH;
    cxChars bytes = allocator->malloc(bytessize);
    AES_cbc_encrypt((const cxUInt8 *)(dataptr + AES_KEY_LENGTH), (cxUInt8 *)bytes, bytessize, &dKey, iv, AES_DECRYPT);
    //check cut bytes
    cxChar bc = bytes[bytessize - 1];
    if(bc > 0 && bc < AES_KEY_LENGTH){
        cxChar bv[bc];
        memset(bv, bc, bc);
        if(memcmp(bv, bytes + bytessize - bc, bc) == 0){
            bytessize -= bc;
        }
    }
    return cxStringAttachMem(bytes, bytessize);
}

cxString cxAESEncode(cxString data,cxString key)
{
    CX_ASSERT(cxStringOK(data) && cxStringOK(key), "args error");
    //make random iv
    cxSetRandSeed();
    cxUChar iv[AES_KEY_LENGTH]={0};
    cxUChar ik[AES_KEY_LENGTH]={0};
    for(cxInt i=0; i < AES_KEY_LENGTH; i++){
        iv[i] = cxRand(0, 255);
    }
    cxInt keylen = cxStringLength(key);
    if(keylen > AES_KEY_LENGTH){
        memcpy(ik, cxStringBody(key), AES_KEY_LENGTH);
    }else {
        memcpy(ik, cxStringBody(key), keylen);
    }
    AES_KEY eKey;
    if(AES_set_encrypt_key((const cxUInt8 *)ik, AES_KEY_LENGTH * 8, &eKey) != 0){
        return NULL;
    }
    cxInt length = cxStringLength(data);
    cxConstChars dataptr = cxStringBody(data);
    //
    cxInt newsize = 0;
    if(length % AES_KEY_LENGTH == 0){
        newsize = length;
    }else{
        newsize = length + AES_KEY_LENGTH - (length % AES_KEY_LENGTH);
    }
    //
    cxInt bc = newsize - length;
    cxInt bytessize = newsize + AES_KEY_LENGTH;
    cxChars bytes = allocator->malloc(bytessize);
    //save iv
    memcpy(bytes, iv, AES_KEY_LENGTH);
    //encode data
    cxChars ptr = bytes + AES_KEY_LENGTH;
    memcpy(ptr, dataptr, length);
    memset(ptr + length, bc, bc);
    //aes cbc
    AES_cbc_encrypt((const cxUInt8 *)ptr, (cxUInt8 *)ptr, newsize, &eKey, iv, AES_ENCRYPT);
    return cxStringAttachMem(bytes, bytessize);
}


cxBool cxStringToBool(cxString str,cxBool dv)
{
    CX_RETURN(!cxStringOK(str), dv);
    return strcmp(str->strptr.d, "true") == 0;
}

cxInt cxStringToInt(cxString str,cxInt dv)
{
    CX_RETURN(!cxStringOK(str), dv);
    return atoi(str->strptr.d);
}

cxDouble cxStringToDouble(cxString str,cxDouble dv)
{
    CX_RETURN(!cxStringOK(str), dv);
    return atof(str->strptr.d);
}

cxLong cxStringToLong(cxString str,cxLong dv)
{
    CX_RETURN(!cxStringOK(str), dv);
    return atol(str->strptr.d);
}

cxString cxMD5(cxString v)
{
    CX_ASSERT(cxStringOK(v), "v args error");
    static const char hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    CX_ASSERT(v != NULL, "args error");
    mongo_md5_state_t state={0};
    mongo_md5_byte_t digest[MD5_DIGEST_LENGTH];
    mongo_md5_init(&state);
    mongo_md5_append(&state, (const mongo_md5_byte_t *)cxStringBody(v), cxStringLength(v));
    mongo_md5_finish(&state, digest);
    cxChar md5[MD5_DIGEST_LENGTH*2 + 1]={0};
    for(cxInt i = 0; i < MD5_DIGEST_LENGTH; i++){
        md5[2*i] = hex[(digest[i] & 0xf0)>> 4];
        md5[2*i + 1] = hex[digest[i] & 0x0f];
    }
    return cxStringConstChars(md5);
}

void cxStringErase(cxString str,cxInt c)
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


void cxStringClear(cxString string)
{
    if(cxStringLength(string) > 0){
        utstring_clear(&string->strptr);
    }
}

cxBool cxStringHasConstChars(cxString str,cxConstChars cs)
{
    CX_ASSERT_THIS(str, cxString);
    if(!cxStringOK(this) || !cxConstCharsOK(cs)){
        return false;
    }
    cxConstChars body = cxStringBody(this);
    return cxConstCharsHas(body, cs);
}

cxBool cxStringEqu(cxString s1,cxString s2)
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

cxString cxStringAttachChars(cxChars str)
{
    CX_ASSERT(str != NULL, "str null");
    return cxStringAttachMem(str, (cxInt)strlen(str));
}

cxString cxStringConstChars(cxConstChars str)
{
    CX_ASSERT(str != NULL, "str null");
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, (void *)str, (cxInt)strlen(str));
    return rv;
}

void cxStringReplace(cxString string,cxChar find,cxChar rep)
{
    cxChars ptr = (cxChars)cxStringBody(string);
    cxInt len = cxStringLength(string);
    for(cxInt i=0; i < len; i++){
        if(ptr[i] == find){
            ptr[i] = rep;
        }
    }
}

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c)
{
    for(cxInt i=0; i < strlen(sp);i++){
        if(sp[i] == c){
            return true;
        }
    }
    return  false;
}

cxArray cxStringSplit(cxString string,cxConstChars sp)
{
    CX_ASSERT(string != NULL, "args error");
    cxConstChars body = cxStringBody(string);
    cxInt length = cxStringLength(string) + 1;
    cxChar buffer[length];
    cxInt idx = 0;
    cxArray ret = CX_CREATE(cxArray);
    for(cxInt i=0; i < length; i++){
        if(!cxConstCharsHasChar(sp, body[i]) && body[i] != '\0'){
            buffer[idx++] = body[i];
            continue;
        }
        if(idx == 0){
            continue;
        }
        cxString item = cxStringBinary(buffer, idx);
        CX_ASSERT(item != NULL, "create string with data error");
        cxArrayAppend(ret, item);
        idx = 0;
    }
    return ret;
}

cxAny cxStringBody(cxString string)
{
    CX_RETURN(string == NULL,NULL);
    return utstring_body(&string->strptr);
}

void cxStringConcat(cxString string,cxString str)
{
    utstring_concat(&string->strptr, &str->strptr);
}

cxInt cxStringLength(cxString string)
{
    CX_RETURN(string == NULL, 0);
    return utstring_len(&string->strptr);
}

cxString cxStringBinary(cxAny d,cxInt l)
{
    cxString rv = CX_CREATE(cxString);
    cxStringAppend(rv, d, l);
    return rv;
}

cxBool cxConstCharsIsNumber(cxConstChars s)
{
    CX_RETURN(s == NULL, false);
    for(cxInt i = 0; i < strlen(s); i++){
        if(!isdigit(s[i])){
            return false;
        }
    }
    return true;
}

cxString cxStringNoCopy(cxAny d,cxInt l)
{
    cxString rv = CX_CREATE(cxString);
    rv->strptr.d = d;
    rv->strptr.i = l;
    rv->strptr.n = l;
    rv->nocopy = true;
    return rv;
}

cxString cxStringAttachMem(cxChars d,cxInt l)
{
    cxString rv = CX_CREATE(cxString);
    rv->strptr.d = d;
    rv->strptr.i = l;
    rv->strptr.n = l;
    return rv;
}

cxString cxStringCreate(cxConstChars format,...)
{
    cxString rv = CX_CREATE(cxString);
    va_list ap;
    va_start(ap, format);
    utstring_printf_va(&rv->strptr, format, ap);
    va_end(ap);
    return rv;
}

void cxStringFormat(cxString string,cxConstChars format,...)
{
    va_list ap;
    va_start(ap, format);
    utstring_printf_va(&string->strptr, format, ap);
    va_end(ap);
}

void cxStringAppendByte(cxString str,cxByte b)
{
    utstring_bincpy(&str->strptr, &b, 1);
}

void cxStringAppend(cxString string,cxConstChars d,cxInt l)
{
    CX_ASSERT(l > 0 && d != NULL, "args error");
    utstring_bincpy(&string->strptr, d, l);
}





