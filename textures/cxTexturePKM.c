//
//  cxTexturePKM.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTexturePKM.h"

typedef struct{
	uint32_t pkmTag;
    uint16_t version;
    uint16_t type;
    uint16_t extWidth;
    uint16_t extHeight;
    uint16_t orgWidth;
    uint16_t orgHeight;
} cxPKMHeader;

#define CX_PKM_TAG (*(uint32_t *)("PKM\x20"))

static cxBool cxTexturePKMLoad(cxAny this,cxStream stream)
{
    cxBool ret = false;
    cxTexturePKM pkm = this;
    CX_ASSERT(stream != NULL, "pvr stream not set");
    if(!cxOpenGLInstance()->support_GL_OES_compressed_ETC1_RGB8_texture){
        CX_ERROR("platform not support etc1 texture");
        return ret;
    }
    if(!cxStreamOpen(stream)){
        CX_ERROR("open pkm stream failed");
        return ret;
    }
    //read pkm head
    cxPKMHeader header;
    cxInt size = cxStreamRead(stream,&header,sizeof(cxPKMHeader));
    if(size != sizeof(cxPKMHeader) || header.pkmTag != CX_PKM_TAG){
        CX_ERROR("read pvr header failed");
        goto completed;
    }
    header.extWidth = CX_SWAP16(header.extWidth);
    header.extHeight = CX_SWAP16(header.extHeight);
    header.orgWidth = CX_SWAP16(header.orgWidth);
    header.orgHeight = CX_SWAP16(header.orgHeight);
    pkm->super.size = cxSize2fv(header.extWidth, header.extHeight);
    cxOpenGLGenTextures(1, &pkm->super.textureId);
    cxOpenGLBindTexture(0, pkm->super.textureId);
    cxInt bufsiz = stream->length - sizeof(cxPKMHeader);
    cxPointer buffer = allocator->malloc(bufsiz);
    cxInt readbytes = cxStreamRead(stream, buffer, bufsiz);
    CX_ASSERT(readbytes == bufsiz, "read pkm data error");
#if defined(GL_OES_compressed_ETC1_RGB8_texture)
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES,header.extWidth, header.extHeight, 0, bufsiz, buffer);
#endif
    allocator->free(buffer);
    cxOpenGLBindTexture(0, 0);
    ret = true;
completed:
    cxStreamClose(stream);
    return ret;
}

static void cxTexturePKMBind(cxAny this)
{
    cxTexturePKM pkm = this;
    cxOpenGLBindTexture(0, pkm->super.textureId);
}

CX_OBJECT_INIT(cxTexturePKM, cxTexture)
{
    CX_METHOD_SET(this->super.Bind, cxTexturePKMBind);
    CX_METHOD_SET(this->super.Load, cxTexturePKMLoad);
}
CX_OBJECT_FREE(cxTexturePKM, cxTexture)
{
    //
}
CX_OBJECT_TERM(cxTexturePKM, cxTexture)

cxTexture cxTexturePKMLoadStream(cxStream stream)
{
    cxTexturePKM pkm = CX_CREATE(cxTexturePKM);
    cxTextureLoad((cxTexture)pkm,stream);
    return (cxTexture)pkm;
}
