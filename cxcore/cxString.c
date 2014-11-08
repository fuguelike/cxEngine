//
//  cxString.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <zlib.h>
#include <algorithm/aes.h>
#include "cxMD5.h"
#include "cxString.h"
#include "cxJson.h"


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

cxString cxZCompressed(cxString data)
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

cxString cxZDecompress(cxString data)
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






