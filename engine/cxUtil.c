//
//  cxUtil.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <zlib.h>
#include <evhttp.h>
#include <sys/time.h>
#include <ctype.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxFileStream.h>
#include <engine/cxEngine.h>
#include <algorithm/aes.h>
#include <sys/stat.h>
#include "cxUtil.h"

#define AES_KEY_LENGTH  16

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
    return cxStringAttach(bytes, bytessize);
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
    if(cxStringLength(key) > AES_KEY_LENGTH){
        memcpy(ik, cxStringBody(key), AES_KEY_LENGTH);
    }else {
        memcpy(ik, cxStringBody(key), cxStringLength(key));
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
    return cxStringAttach(bytes, bytessize);
}

cxBool cxDocumentExists(cxConstChars file)
{
    cxString path = cxDocumentPath(file);
    CX_RETURN(path == NULL, false);
    struct stat stat={0};
    return lstat(cxStringBody(path), &stat) == 0;
}

cxBool cxCopyFile(cxConstChars file,cxCopyFileFunc func,cxAny udata)
{
    cxStream src = cxAssetsStreamCreate(file);
    cxStream dst = cxFileStreamCreate(file, false);
    if(!cxStreamOpen(src)){
        return false;
    }
    if(!cxStreamOpen(dst)){
        return false;
    }
    cxChar buffer[4096];
    cxProgress p = {0};
    p.current = 0;
    p.total = src->length;
    while (true) {
        cxInt rbytes = cxStreamRead(src, buffer, 4096);
        if(rbytes <= 0){
            break;
        }
        p.current += rbytes;
        cxInt wbytes = cxStreamWrite(dst, buffer, rbytes);
        CX_ASSERT(wbytes == rbytes, "rw error");
        if(func != NULL){
            func(file,&p,udata);
        }
        CX_UNUSED_PARAM(wbytes);
    }
    return true;
}

cxUInt cxBinaryToUInt(cxConstChars bs)
{
    CX_ASSERT(bs != NULL, "args error");
    cxInt len = strlen(bs);
    cxUInt ret = 0;
    for(cxInt i=0; i < len; i++){
        CX_ASSERT(bs[i] == '0' || bs[i] == '1', "str error");
        if(bs[i] != '1'){
            continue;
        }
        ret |= (1 << (len - i - 1));
    }
    return ret;
}

cxUInt cxHexToUInt(cxConstChars bs)
{
    CX_ASSERT(bs != NULL, "args error");
    cxInt len = strlen(bs);
    cxUInt ret = 0;
    for(cxInt i=0; i < len; i++){
        cxChar v = tolower(bs[i]);
        cxUInt tmp = 0;
        if(v >= '0' && v <= '9'){
            tmp = v - '0';
        }else if(v >= 'a' && v <= 'f'){
            tmp = v - 'a' + 0xa;
        }else{
            CX_ASSERT(false, "str error");
        }
        ret |= (tmp << ((len - i - 1)) * 4);
    }
    return ret;
}

cxAudioFileType cxAudioGetType(cxConstChars file)
{
    cxConstChars ext = strrchr(file, '.');
    CX_RETURN(ext == NULL,cxAudioFileTypeNone);
    if(cxConstCharsEqu(ext, ".mp3")){
        return cxAudioFileTypeMP3;
    }
    if(cxConstCharsEqu(ext, ".wav")){
        return cxAudioFileTypeWAV;
    }
    return cxAudioFileTypeNone;
}

cxString cxCompressed(cxString data)
{
    CX_ASSERT(data != NULL, "args error");
    cxInt datasiz = cxStringLength(data);
    uLongf size = compressBound(datasiz);
    char *cv = allocator->malloc(size);
    if(compress((Bytef *)cv, &size, (Bytef *)cxStringBody(data), datasiz) == Z_OK){
        return cxStringAttach(cv, size);
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
        return cxStringAttach(cv, size);
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

cxDouble cxTimestamp()
{
    struct timeval val = {0};
    gettimeofday(&val, NULL);
    return val.tv_sec + (cxDouble)val.tv_usec/(cxDouble)1000000.0;
}