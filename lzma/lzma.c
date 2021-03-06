//
//  lzma.c
//  cxEngineStatic
//
//  Created by xuhua on 10/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "lzma.h"
//16bytes
typedef struct{
    int32_t dataLen;                //not include head  4bytes
    unsigned char props[LZMA_PROPS_SIZE]; //props   5 bytes
    char flags[7];                  //  7bytes
}LzmaDataHead;

static char lzmaFlags[7]={0};
void LzmaSetFlags(char *flags)
{
    memcpy(lzmaFlags, flags, 7);
}

int LzmaGetCompressLen(int srcLen)
{
    return srcLen * 1.1f + LZMA_PROPS_SIZE + sizeof(LzmaDataHead);
}

char *LzmaCompressImp(const char *psrc,int asrcLen,char *pdst,int *pdstLen)
{
    assert(psrc != NULL && asrcLen > 0 && pdstLen != NULL && pdst != NULL);
    const unsigned char *src = (const unsigned char *)psrc;
    size_t srcLen = asrcLen;
    unsigned char outProps[LZMA_PROPS_SIZE]={0};
    size_t outPropsLen = LZMA_PROPS_SIZE;
    size_t dstLen = *pdstLen;
    unsigned char *dst = (unsigned char *)(pdst + sizeof(LzmaDataHead));
    int ret = LzmaCompress(dst, &dstLen, src, srcLen, outProps, &outPropsLen, 5, 1 << 16, 3, 0, 2, 32, 1);
    if(ret != SZ_OK){
        free(pdst);
        return NULL;
    }
    LzmaDataHead *head = (LzmaDataHead *)pdst;
    head->dataLen = (int32_t)srcLen;
    memcpy(head->props, outProps, outPropsLen);
    memcpy(head->flags, lzmaFlags, 7);
    *pdstLen = (int)(dstLen + sizeof(LzmaDataHead));
    return (char *)pdst;
}

int LzmaGetUncompressLen(void *src)
{
    assert(src != NULL);
    LzmaDataHead *head = (LzmaDataHead *)src;
    return head->dataLen;
}

char *LzmaUncompressImp(const char *psrc,int asrcLen,char *pdst,int *pdstLen)
{
    assert(psrc != NULL && asrcLen > 0 && pdstLen != NULL && pdst != NULL);
    const unsigned char *src = (const unsigned char *)psrc;
    LzmaDataHead *head = (LzmaDataHead *)src;
    if(memcmp(head->flags, lzmaFlags, 7) != 0){
        return NULL;
    }
    size_t srcLen = asrcLen - sizeof(LzmaDataHead);
    assert(*pdstLen == head->dataLen);
    size_t dstLen = (size_t)head->dataLen;
    //max 128M
    if(dstLen > 1024*1024*128 || dstLen <= 0){
        return NULL;
    }
    int ret = LzmaUncompress((unsigned char *)pdst, &dstLen, src + sizeof(LzmaDataHead), &srcLen, head->props, LZMA_PROPS_SIZE);
    if(ret != SZ_OK){
        return NULL;
    }
    *pdstLen = (int)dstLen;
    return (char *)pdst;
}



