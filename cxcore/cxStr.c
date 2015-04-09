//
//  cxStr.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <zlib.h>
#include <algorithm/aes.h>
#include "cxMD5.h"
#include "cxStr.h"
#include "cxJson.h"
#include "xxtea.h"
#include "utf8.h"


CX_SETTER_DEF(cxStr, string)
{
    cxConstChars str = cxJsonToConstChars(value);
    if(str == NULL){
        return;
    }
    cxStrFormat(this, "%s",str);
}
CX_SETTER_DEF(cxStr, number)
{
    if(cxJsonIsInt(value)){
        cxStrFormat(this, "%d",cxJsonToInt(value, 0));
    }else if(cxJsonIsDouble(value)){
        cxStrFormat(this, "%f",cxJsonToDouble(value, 0));
    }else if(cxJsonIsBool(value)){
        cxStrFormat(this, "%s", cxJsonToBool(value, false) ? "true" : "false");
    }
}

CX_TYPE(cxStr, cxObject)
{
    CX_SETTER(cxStr, string);
    CX_SETTER(cxStr, number);
}
CX_INIT(cxStr, cxObject)
{
    this->Encode = cxStrEncodeUTF8;
}
CX_FREE(cxStr, cxObject)
{
    if(!this->nocopy){
        utstring_done(&this->strptr);
    }
}
CX_TERM(cxStr, cxObject)

static cxStr emptyString = NULL;

cxStr cxStrEmpty()
{
    if(emptyString == NULL){
        CX_RETAIN_SET(emptyString, cxStrConstChars(""));
    }
    return emptyString;
}

cxBool cxStrIsEmpty(cxStr str)
{
    return str == cxStrEmpty();
}

cxAny cxStrBody(cxStr string)
{
    CX_RETURN(string == NULL,NULL);
    return utstring_body(&string->strptr);
}

cxHashKey cxStrHashKey(cxStr str)
{
    cxHashKey key;
    key.data = cxStrBody(str);
    key.length = cxStrLength(str);
    return key;
}

void cxStrConcat(cxStr string,cxStr str)
{
    CX_ASSERT(string != NULL && str != NULL, "args error");
    utstring_concat(&string->strptr, &str->strptr);
}

cxInt cxStrLength(cxStr string)
{
    CX_RETURN(string == NULL, 0);
    return utstring_len(&string->strptr);
}

void cxStrAppend(cxStr string,cxConstChars d,cxInt l)
{
    CX_ASSERT(l > 0 && d != NULL, "args error");
    utstring_bincpy(&string->strptr, d, l);
}

cxStr cxStrBinary(cxAny d,cxInt l)
{
    CX_ASSERT(d != NULL && l > 0, "args error");
    cxStr rv = CX_CREATE(cxStr);
    cxStrAppend(rv, d, l);
    return rv;
}

void cxStrPrint(cxStr str)
{
    if(cxStrOK(str)){
        CX_LOGGER("%s",cxStrBody(str));
    }
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

cxInt cxStrUTF8Length(cxStr str)
{
    if(!cxStrOK(str)){
        return 0;
    }
    return getUTF8StrLength(cxStrBody(str));
}

//path[512]={0};
cxInt cxConstCharsTypePath(cxConstChars s,cxChars path)
{
    cxInt ret = 0;
    if(s == NULL){
        return ret;
    }
    cxInt len = cxConstCharsLength(s);
    if(len < 4 || s[0] != '$'){
        memcpy(path, s, len);
        return 1;
    }
    if(s[1] == '(' && s[len - 1] == ')'){
        //$(id)
        ret = 2;
        memcpy(path, s + 2, len - 3);
    }else if(s[1] == '{' && s[len - 1] == '}'){
        //${id}
        ret = 3;
        memcpy(path, s + 2, len - 3);
    }else{
        ret = 1;
        memcpy(path, s, len);
    }
    return ret;
}

cxStr cxStrNoCopy(cxAny d,cxInt l)
{
    CX_ASSERT(d != NULL && l > 0, "args error");
    cxStr rv = CX_CREATE(cxStr);
    rv->strptr.d = d;
    rv->strptr.i = l;
    rv->strptr.n = l;
    rv->nocopy = true;
    return rv;
}

cxStr cxStrAttachMem(cxChars d,cxInt l)
{
    CX_ASSERT(d != NULL && l > 0, "args error");
    cxStr rv = CX_CREATE(cxStr);
    rv->strptr.d = d;
    rv->strptr.i = l;
    rv->strptr.n = l;
    return rv;
}

CX_INLINE cxStr cxStrAttachMemWithEncode(cxChars d,cxInt l,cxStrEncode encode)
{
    CX_ASSERT(d != NULL && l > 0, "args error");
    cxStr rv = CX_CREATE(cxStr);
    rv->strptr.d = d;
    rv->strptr.i = l;
    rv->strptr.n = l;
    rv->Encode = encode;
    return rv;
}

cxStr cxStrCreate(cxConstChars format,...)
{
    CX_ASSERT(format != NULL, "args error");
    cxStr rv = CX_CREATE(cxStr);
    va_list ap;
    va_start(ap, format);
    utstring_printf_va(&rv->strptr, format, ap);
    va_end(ap);
    return rv;
}

void cxStrFormatVA(cxStr string,cxConstChars format,va_list ap)
{
    CX_ASSERT(string != NULL && format != NULL, "args error");
    utstring_printf_va(&string->strptr, format, ap);
}

void cxStrFormat(cxStr string,cxConstChars format,...)
{
    CX_ASSERT(string != NULL && format != NULL, "args error");
    va_list ap;
    va_start(ap, format);
    cxStrFormatVA(string, format, ap);
    va_end(ap);
}

void cxStrAppendByte(cxStr str,cxByte b)
{
    CX_ASSERT(str != NULL, "args error");
    cxInt l = sizeof(cxByte);
    utstring_bincpy(&str->strptr, &b, l);
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

cxBool cxStrToBool(cxStr str,cxBool dv)
{
    CX_RETURN(!cxStrOK(str), dv);
    return strcmp(str->strptr.d, "true") == 0;
}

cxInt cxStrToInt(cxStr str,cxInt dv)
{
    CX_RETURN(!cxStrOK(str), dv);
    return atoi(str->strptr.d);
}

cxDouble cxStrToDouble(cxStr str,cxDouble dv)
{
    CX_RETURN(!cxStrOK(str), dv);
    return atof(str->strptr.d);
}

cxLong cxStrToLong(cxStr str,cxLong dv)
{
    CX_RETURN(!cxStrOK(str), dv);
    return atol(str->strptr.d);
}

void cxStrClear(cxStr string)
{
    if(cxStrLength(string) > 0){
        utstring_clear(&string->strptr);
    }
}

void cxStrErase(cxStr str,cxInt c)
{
    cxInt l = cxStrLength(str);
    CX_RETURN(c == 0 || l < abs(c));
    cxInt r = l - abs(c);
    if(r == 0){
        cxStrClear(str);
        return;
    }
    if(c > 0){
        memmove(str->strptr.d, str->strptr.d + c, r);
    }
    str->strptr.i = r;
    str->strptr.d[r] = '\0';
}

cxBool cxStrHasPrefix(cxStr str,cxConstChars prefix)
{
    CX_ASSERT_THIS(str, cxStr);
    cxInt len = (cxInt)strlen(prefix);
    if(this->strptr.i < len){
        return false;
    }
    return memcmp(this->strptr.d, prefix, len) == 0;
}

cxBool cxStrHasSuffix(cxStr str,cxConstChars suffix)
{
    CX_ASSERT_THIS(str, cxStr);
    cxInt len = (cxInt)strlen(suffix);
    if(this->strptr.i < len){
        return false;
    }
    return memcmp(this->strptr.d + this->strptr.i - len, suffix, len) == 0;
}

cxBool cxStrHasConstChars(cxStr str,cxConstChars cs)
{
    if(!cxStrOK(str) || !cxConstCharsOK(cs)){
        return false;
    }
    cxConstChars body = cxStrBody(str);
    return cxConstCharsHas(body, cs);
}

cxBool cxStrHasChar(cxStr str,cxChar c)
{
    return cxStrOK(str) && cxConstCharsHasChar(cxStrBody(str), c);
}

cxBool cxStrHasChars(cxStr str,cxChars cs)
{
    if(!cxStrOK(str)){
        return false;
    }
    cxInt num = (cxInt)strlen(cs);
    for(cxInt i=0; i < num;i++){
        if(cxStrHasChar(str, cs[i])){
            return true;
        }
    }
    return false;
}

cxBool cxStrEqu(cxStr s1,cxStr s2)
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

cxStr cxStrAttachChars(cxChars str)
{
    CX_ASSERT(str != NULL, "str null");
    return cxStrAttachMem(str, (cxInt)strlen(str));
}

cxStr cxStrConstChars(cxConstChars str)
{
    CX_ASSERT(str != NULL, "str null");
    cxStr rv = CX_CREATE(cxStr);
    cxStrAppend(rv, (void *)str, (cxInt)strlen(str));
    return rv;
}

void cxStrReplace(cxStr string,cxChar find,cxChar rep)
{
    cxChars ptr = (cxChars)cxStrBody(string);
    cxInt len = cxStrLength(string);
    for(cxInt i=0; i < len; i++){
        if(ptr[i] == find){
            ptr[i] = rep;
        }
    }
}

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c)
{
    for(cxInt i=0; i < (cxInt)strlen(sp);i++){
        if(sp[i] == c){
            return true;
        }
    }
    return  false;
}


cxStr cxZCompressed(cxStr data)
{
    CX_ASSERT(data != NULL, "args error");
    cxInt datasiz = cxStrLength(data);
    uLongf size = compressBound(datasiz);
    char *cv = allocator->malloc(size);
    if(compress((Bytef *)cv, &size, (Bytef *)cxStrBody(data), datasiz) == Z_OK){
        return cxStrAttachMem(cv, (cxInt)size);
    }
    allocator->free(cv);
    return NULL;
}

cxStr cxZDecompress(cxStr data)
{
    CX_ASSERT(data != NULL, "args error");
    cxInt datasiz = cxStrLength(data);
    uLongf size = datasiz * 4;
    char *cv = allocator->malloc(size);
    int ret = uncompress((Bytef *)cv, &size, (Bytef *)cxStrBody(data), (uLongf)datasiz);
    while(ret == Z_BUF_ERROR){
        size = (size << 1);
        cv = allocator->realloc(cv,size);
        ret = uncompress((Bytef *)cv, &size, (Bytef *)cxStrBody(data), (uLongf)datasiz);
    }
    if(ret == Z_OK){
        return cxStrAttachMem(cv, (cxInt)size);
    }
    allocator->free(cv);
    return NULL;
}

cxStr cxTEAEncode(cxStr data,cxStr key)
{
    CX_ASSERT(cxStrOK(data) && cxStrOK(key), "args error");
    xxtea_long rl = 0;
    unsigned char *pd = cxStrBody(data);
    xxtea_long pl = cxStrLength(data);
    unsigned char *kd = cxStrBody(key);
    xxtea_long kl = cxStrLength(key);
    unsigned char *rp = xxtea_encrypt(pd, pl, kd, kl, &rl);
    if(rp == NULL){
        return NULL;
    }
    return cxStrAttachMem((cxChars)rp, rl);
}

cxStr cxTEADecode(cxStr data,cxStr key)
{
    CX_ASSERT(cxStrOK(data) && cxStrOK(key), "args error");
    xxtea_long rl = 0;
    unsigned char *pd = cxStrBody(data);
    xxtea_long pl = cxStrLength(data);
    unsigned char *kd = cxStrBody(key);
    xxtea_long kl = cxStrLength(key);
    unsigned char *rp = xxtea_decrypt(pd, pl, kd, kl, &rl);
    if(rp == NULL){
        return NULL;
    }
    return cxStrAttachMem((cxChars)rp, rl);
}

cxStr cxAESDecode(cxStr data,cxStr key)
{
    CX_ASSERT(cxStrOK(data) && cxStrOK(key), "args error");
    cxUChar iv[AES_KEY_LENGTH]={0};
    cxUChar ik[AES_KEY_LENGTH]={0};
    if(cxStrLength(key) > AES_KEY_LENGTH){
        memcpy(ik, cxStrBody(key), AES_KEY_LENGTH);
    }else {
        memcpy(ik, cxStrBody(key), cxStrLength(key));
    }
    AES_KEY dKey;
    if(AES_set_decrypt_key((const cxUInt8 *)ik, AES_KEY_LENGTH*8, &dKey) != 0){
        return NULL;
    }
    //
    cxConstChars dataptr = cxStrBody(data);
    cxInt length = cxStrLength(data);
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
    return cxStrAttachMem(bytes, bytessize);
}

cxStr cxAESEncode(cxStr data,cxStr key)
{
    CX_ASSERT(cxStrOK(data) && cxStrOK(key), "args error");
    //make random iv
    cxSetRandSeed();
    cxUChar iv[AES_KEY_LENGTH]={0};
    cxUChar ik[AES_KEY_LENGTH]={0};
    for(cxInt i=0; i < AES_KEY_LENGTH; i++){
        iv[i] = cxRand(0, 255);
    }
    cxInt keylen = cxStrLength(key);
    if(keylen > AES_KEY_LENGTH){
        memcpy(ik, cxStrBody(key), AES_KEY_LENGTH);
    }else {
        memcpy(ik, cxStrBody(key), keylen);
    }
    AES_KEY eKey;
    if(AES_set_encrypt_key((const cxUInt8 *)ik, AES_KEY_LENGTH * 8, &eKey) != 0){
        return NULL;
    }
    cxInt length = cxStrLength(data);
    cxConstChars dataptr = cxStrBody(data);
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
    return cxStrAttachMem(bytes, bytessize);
}

cxStr cxStrToUTF8(cxStr str)
{
    CX_ASSERT(cxStrOK(str), "str null error");
    cxAny sptr = cxStrBody(str);
    cxInt len = cxStrLength(str);
    //ConvertUTF16toUTF8
    if(str->Encode == cxStrEncodeUTF16){
        cxInt dlen = len * 2 + 1;
        cxAny buffer = allocator->malloc(dlen);
        UTF8 *dstart = buffer;
        ConversionResult ret = ConvertUTF16toUTF8((const UTF16 **)&sptr, sptr + len, &dstart, dstart + dlen, strictConversion);
        dlen = (cxInt)((cxLong)dstart - (cxLong)buffer);
        if(ret == conversionOK)return cxStrAttachMemWithEncode(buffer, dlen,cxStrEncodeUTF8);
        allocator->free(buffer);
        return NULL;
    }
    //ConvertUTF32toUTF8;
    if(str->Encode == cxStrEncodeUTF32){
        cxInt dlen = len + 1;
        cxAny buffer = allocator->malloc(dlen);
        UTF8 *dstart = buffer;
        ConversionResult ret = ConvertUTF32toUTF8((const UTF32 **)&sptr, sptr + len, &dstart, dstart + dlen, strictConversion);
        dlen = (cxInt)((cxLong)dstart - (cxLong)buffer);
        if(ret == conversionOK)return cxStrAttachMemWithEncode(buffer, dlen,cxStrEncodeUTF8);
        allocator->free(buffer);
        return NULL;
    }
    return str;
}

cxStr cxStrToUTF16(cxStr str)
{
    CX_ASSERT(cxStrOK(str), "str null error");
    cxAny sptr = cxStrBody(str);
    cxInt len = cxStrLength(str);
    //ConvertUTF8toUTF16
    if(str->Encode == cxStrEncodeUTF8){
        cxInt dlen = len * 2 + 1;
        cxAny buffer = allocator->malloc(dlen);
        UTF16 *dstart = buffer;
        ConversionResult ret = ConvertUTF8toUTF16((const UTF8 **)&sptr, sptr + len, &dstart, dstart + dlen, strictConversion);
        dlen = (cxInt)((cxLong)dstart - (cxLong)buffer);
        if(ret == conversionOK)return cxStrAttachMemWithEncode(buffer, dlen,cxStrEncodeUTF16);
        allocator->free(buffer);
        return NULL;
    }
    //ConvertUTF32toUTF16
    if(str->Encode == cxStrEncodeUTF32){
        cxInt dlen = len + 1;
        cxAny buffer = allocator->malloc(dlen);
        UTF16 *dstart = buffer;
        ConversionResult ret = ConvertUTF32toUTF16((const UTF32 **)&sptr, sptr + len, &dstart, dstart + dlen, strictConversion);
        dlen = (cxInt)((cxLong)dstart - (cxLong)buffer);
        if(ret == conversionOK)return cxStrAttachMemWithEncode(buffer, dlen,cxStrEncodeUTF16);
        allocator->free(buffer);
        return NULL;
    }
    return str;
}

cxStr cxStrToUTF32(cxStr str)
{
    CX_ASSERT(cxStrOK(str), "str null error");
    cxAny sptr = cxStrBody(str);
    cxInt len = cxStrLength(str);
    //ConvertUTF8toUTF32
    if(str->Encode == cxStrEncodeUTF8){
        cxInt dlen = len * 4 + 1;
        cxAny buffer = allocator->malloc(dlen);
        UTF32 *dstart = buffer;
        ConversionResult ret = ConvertUTF8toUTF32((const UTF8 **)&sptr, sptr + len, &dstart, dstart + dlen, strictConversion);
        dlen = (cxInt)((cxLong)dstart - (cxLong)buffer);
        if(ret == conversionOK)return cxStrAttachMemWithEncode(buffer, dlen,cxStrEncodeUTF32);
        allocator->free(buffer);
        return NULL;
    }
    //ConvertUTF16toUTF32
    if(str->Encode == cxStrEncodeUTF16){
        cxInt dlen = len * 2 + 1;
        cxAny buffer = allocator->malloc(dlen);
        UTF32 *dstart = buffer;
        ConversionResult ret = ConvertUTF16toUTF32((const UTF16 **)&sptr, sptr + len, &dstart, dstart + dlen, strictConversion);
        dlen = (cxInt)((cxLong)dstart - (cxLong)buffer);
        if(ret == conversionOK)return cxStrAttachMemWithEncode(buffer, dlen,cxStrEncodeUTF32);
        allocator->free(buffer);
        return NULL;
    }
    return str;
}

cxStr cxMD5(cxStr v)
{
    CX_ASSERT(cxStrOK(v), "v args error");
    static const char hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    CX_ASSERT(v != NULL, "args error");
    mongo_md5_state_t state={0};
    mongo_md5_byte_t digest[MD5_DIGEST_LENGTH];
    mongo_md5_init(&state);
    mongo_md5_append(&state, (const mongo_md5_byte_t *)cxStrBody(v), cxStrLength(v));
    mongo_md5_finish(&state, digest);
    cxChar md5[MD5_DIGEST_LENGTH*2 + 1]={0};
    for(cxInt i = 0; i < MD5_DIGEST_LENGTH; i++){
        md5[2*i] = hex[(digest[i] & 0xf0)>> 4];
        md5[2*i + 1] = hex[digest[i] & 0x0f];
    }
    return cxStrConstChars(md5);
}

cxArray cxStrSplit(cxStr string,cxConstChars sp)
{
    CX_ASSERT(string != NULL, "args error");
    cxConstChars body = cxStrBody(string);
    cxInt length = cxStrLength(string) + 1;
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
        cxStr item = cxStrBinary(buffer, idx);
        CX_ASSERT(item != NULL, "create string with data error");
        cxArrayAppend(ret, item);
        idx = 0;
    }
    return ret;
}






