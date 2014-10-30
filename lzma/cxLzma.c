//
//  cxLzma.c
//  cxEngineStatic
//
//  Created by xuhua on 10/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "lzma.h"
#include "cxLzma.h"

void cxLzmaSetFlags(cxString data)
{
    CX_ASSERT(cxStringLength(data) >= 7, "must > 7 bytes");
    LzmaSetFlags((char *)cxStringBody(data));
}

cxString cxLzmaCompress(cxString data)
{
    CX_ASSERT(cxStringOK(data), "data error");
    int len = LzmaGetCompressLen(cxStringLength(data));
    char *ret = allocator->malloc(len);
    if(LzmaCompressImp(cxStringBody(data), cxStringLength(data), ret, &len) == NULL){
        allocator->free(ret);
        return NULL;
    }
    return cxStringAttachMem(ret, len);
}

cxString cxLzmaUncompress(cxString data)
{
    CX_ASSERT(cxStringOK(data), "data error");
    int len = LzmaGetUncompressLen((cxAny)cxStringBody(data));
    char *ret = allocator->malloc(len);
    if(LzmaUncompressImp(cxStringBody(data), cxStringLength(data), ret,&len) == NULL){
        allocator->free(ret);
        return NULL;
    }
    return cxStringAttachMem(ret, len);
}