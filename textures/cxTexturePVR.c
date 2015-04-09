//
//  cxTexturePVR.c
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxTexturePVR.h"

#pragma pack(1)

typedef struct{
	cxUInt32 headerLength;
	cxUInt32 height;
	cxUInt32 width;
	cxUInt32 numMipmaps;
	cxUInt32 flags;
	cxUInt32 dataLength;
	cxUInt32 bpp;
	cxUInt32 bitmaskRed;
	cxUInt32 bitmaskGreen;
	cxUInt32 bitmaskBlue;
	cxUInt32 bitmaskAlpha;
	cxUInt32 pvrTag;
	cxUInt32 numSurfs;
} cxPVRHeader;

// v2
enum{
	cxFormatPVR2BPP = 24,
	cxFormatPVR4BPP,
};

#define CX_PVR_TAG (*(cxUInt32 *)("PVR!"))

#define CX_PVR_TEXTURE_FLAG_TYPE_MASK   0xff

#define CX_PVR_MAX_TABLE_ELEMENTS       16

#pragma pack()

CX_METHOD_DEF(cxTexturePVR,Load,cxBool,cxStream stream)
{
    cxBool ret = false;
    CX_ASSERT(stream != NULL, "pvr stream not set");
    if(!cxOpenGLInstance()->support_GL_IMG_texture_compression_pvrtc){
        CX_ERROR("platform not support pvr texture");
        return ret;
    }
    if(!cxStreamOpen(stream)){
        CX_ERROR("open prv stream failed");
        return ret;
    }
    //read pvr head
    cxPVRHeader header={0};
    cxInt size = cxStreamRead(stream, &header, sizeof(cxPVRHeader));
    if(size != sizeof(cxPVRHeader) ||
       header.headerLength != sizeof(cxPVRHeader) ||
       header.pvrTag != CX_PVR_TAG){
        CX_ERROR("read pvr header failed");
        goto completed;
    }
    cxUInt32 flags = header.flags;
    this->format = flags & CX_PVR_TEXTURE_FLAG_TYPE_MASK;
    if(this->format != cxFormatPVR2BPP && this->format != cxFormatPVR4BPP){
        CX_ERROR("pvr only support 2bpp and 4bpp format");
        goto completed;
    }
    this->cxTexture.size = cxSize2fv(header.width, header.height);
    cxBool hasAlpha = header.bitmaskAlpha;
    this->cxTexture.hasAlpha = hasAlpha;
    if(this->format == cxFormatPVR2BPP){
        this->glFormat = hasAlpha?GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
    }else{
        this->glFormat = hasAlpha?GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
    }
    cxOpenGLGenTextures(1, &this->cxTexture.textureId);
    cxOpenGLBindTexture(this->cxTexture.textureId, 0);
    cxOpenGLSetTexParameters(this->cxTexture.texParam);
    cxUInt dataLen = header.dataLength;
    cxUInt dataOff = 0;
    cxUInt width = header.width;
    cxUInt height = header.height;
    cxUInt blockSize = 0;
    cxUInt bpp = 4;
    cxUInt widthBlocks = 0;
    cxUInt heightBlocks = 0;
    cxInt i = 0;
    cxAny buffer = NULL;
    cxInt bufBytes = 0;
    while (dataOff< dataLen){
        if (this->format == cxFormatPVR4BPP){
            blockSize = 4 * 4;
            widthBlocks = width / 4;
            heightBlocks = height / 4;
            bpp = 4;
        }else{
            blockSize = 8 * 4;
            widthBlocks = width / 8;
            heightBlocks = height / 4;
            bpp = 2;
        }
        if (widthBlocks < 2){
            widthBlocks = 2;
        }
        if (heightBlocks < 2){
            heightBlocks = 2;
        }
        cxUInt bufSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
        if(bufSize > bufBytes){
            buffer = allocator->realloc(buffer,bufSize);
            CX_ASSERT(buffer != NULL, "realloc mem failed");
            bufBytes = bufSize;
        }
        cxInt size = cxStreamRead(stream, buffer, bufSize);
        if(size == bufSize){
            glCompressedTexImage2D(GL_TEXTURE_2D, i, this->glFormat, width, height, 0, bufSize, buffer);
        }else{
            CX_WARN("pvr read stream data failed");
        }
        dataOff += bufSize;
        width = CX_MAX(width >> 1, 1);
        height = CX_MAX(height >> 1, 1);
        i++;
    }
    allocator->free(buffer);
    if(i > 1){
        cxTextureSetParam(this, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    }
    this->cxTexture.hasMipmap = i > 1;
    cxOpenGLBindTexture(0,0);
    ret = true;
completed:
    cxStreamClose(stream);
    return ret;
}
CX_TYPE(cxTexturePVR, cxTexture)
{
    CX_METHOD(cxTexturePVR, Load);
}
CX_INIT(cxTexturePVR, cxTexture)
{
    
}
CX_FREE(cxTexturePVR, cxTexture)
{
    
}
CX_TERM(cxTexturePVR, cxTexture)

cxTexture cxTexturePVRLoadStream(cxStream stream)
{
    cxTexture pvr = CX_CREATE(cxTexturePVR);
    CX_CALL(pvr, Load, CX_M(cxBool,cxStream),stream);
    return pvr;
}


