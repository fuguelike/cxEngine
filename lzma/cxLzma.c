//
//  cxLzma.c
//  cxEngineStatic
//
//  Created by xuhua on 10/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "lzma.h"
#include "cxLzma.h"

void cxLzmaSetFlags(cxStr data)
{
    CX_ASSERT(cxStrLength(data) >= 7, "must > 7 bytes");
    LzmaSetFlags((char *)cxStrBody(data));
}

cxStr cxLzmaCompressed(cxStr data)
{
    CX_ASSERT(cxStrOK(data), "data error");
    int len = LzmaGetCompressLen(cxStrLength(data));
    char *ret = allocator->malloc(len);
    if(LzmaCompressImp(cxStrBody(data), cxStrLength(data), ret, &len) == NULL){
        allocator->free(ret);
        return NULL;
    }
    return cxStrAttachMem(ret, len);
}

cxStr cxLzmaDecompress(cxStr data)
{
    CX_ASSERT(cxStrOK(data), "data error");
    int len = LzmaGetUncompressLen((cxAny)cxStrBody(data));
    char *ret = allocator->malloc(len);
    if(LzmaUncompressImp(cxStrBody(data), cxStrLength(data), ret,&len) == NULL){
        allocator->free(ret);
        return NULL;
    }
    return cxStrAttachMem(ret, len);
}