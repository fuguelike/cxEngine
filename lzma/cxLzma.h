//
//  cxLzma.h
//  cxEngine
//
//  Created by xuhua on 10/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLzma_h
#define cxEngine_cxLzma_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

#pragma pack(1)

enum{
    cxTextureXTFBlockFormat_RGBA8888,
    cxTextureXTFBlockFormat_RGB888,
    cxTextureXTFBlockFormat_ALPHA
}cxTextureXTFBlockFormt;

typedef struct {
    cxUInt8  format;
    cxUInt16 width;
    cxUInt16 height;
    cxUInt32 rgbaLen; //rgba lzma length
}cxTextureXTFBlock;

typedef struct {
    cxUInt32 flags;   //flags = 0x80152901
    cxUInt32 jsonLen; //json data
    cxUInt16 rgbaNum; //cxTextureXTFBlock num
}cxTextureXTFHeader;

typedef struct {
    cxUInt8 type;       //type 1-pvr 2-pkm
    cxUInt16 count;     //file count
    cxUInt32 length;    //file length
}cxTextureMTFHeader;

#pragma pack()

void cxLzmaSetFlags(cxStr flags);

cxStr cxLzmaCompressed(cxStr data);

cxStr cxLzmaDecompress(cxStr data);

CX_C_END

#endif
