//
//  cxTexturePKM.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxUtil.h>
#include "cxTexturePKM.h"
#include "cxTextureFactory.h"

typedef struct{
	cxUInt32 pkmTag;
    cxUInt16 version;
    cxUInt16 type;
    cxUInt16 extWidth;
    cxUInt16 extHeight;
    cxUInt16 orgWidth;
    cxUInt16 orgHeight;
} cxPKMHeader;

#define CX_PKM_TAG  (*(cxUInt32 *)("PKM\x20"))
#define CX_PKM_V1   (*(cxUInt16 *)("10"))
#define CX_PKM_V2   (*(cxUInt16 *)("20"))

static cxBool cxTexturePKMLoad(cxAny ptex,cxStream stream)
{
    CX_ASSERT_THIS(ptex, cxTexturePKM);
    cxBool ret = false;
    CX_ASSERT(stream != NULL, "pkm stream not set");
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
        CX_ERROR("read pkm header failed");
        goto completed;
    }
    header.extWidth = CX_SWAP16(header.extWidth);
    header.extHeight = CX_SWAP16(header.extHeight);
    header.orgWidth = CX_SWAP16(header.orgWidth);
    header.orgHeight = CX_SWAP16(header.orgHeight);
    this->cxTexture.size = cxSize2fv(header.extWidth, header.extHeight);
    if(header.version != CX_PKM_V1){
        CX_ERROR("PKM only use V1 version");
        goto completed;
    }
    cxOpenGLGenTextures(1, &this->cxTexture.textureId);
    cxOpenGLBindTexture(this->cxTexture.textureId, 0);
    cxOpenGLSetTexParameters(this->cxTexture.texParam);
    cxInt bufsiz = stream->length - sizeof(cxPKMHeader);
    cxAny buffer = allocator->malloc(bufsiz);
    cxInt readbytes = cxStreamRead(stream, buffer, bufsiz);
    CX_ASSERT(readbytes == bufsiz, "read pkm data error");
    CX_UNUSED_PARAM(readbytes);
#if defined(GL_OES_compressed_ETC1_RGB8_texture)
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES,header.extWidth, header.extHeight, 0, bufsiz, buffer);
    ret = true;
#else
    CX_ERROR("platform not support GL_OES_compressed_ETC1_RGB8_texture");
    ret = false;
#endif
    allocator->free(buffer);
    cxOpenGLBindTexture(0, 0);
completed:
    cxStreamClose(stream);
    return ret;
}

static void cxTexturePKMBind(cxAny this)
{
    cxTexturePKM pkm = this;
    cxOpenGLBindTexture(pkm->cxTexture.textureId, 0);
}

CX_OBJECT_TYPE(cxTexturePKM, cxTexture)
{
    
}
CX_OBJECT_INIT(cxTexturePKM, cxTexture)
{
    CX_METHOD_SET(this->cxTexture.Bind, cxTexturePKMBind);
    CX_METHOD_SET(this->cxTexture.Load, cxTexturePKMLoad);
}
CX_OBJECT_FREE(cxTexturePKM, cxTexture)
{

}
CX_OBJECT_TERM(cxTexturePKM, cxTexture)

cxTexture cxTexturePKMLoadStream(cxStream stream)
{
    cxTexturePKM pkm = CX_CREATE(cxTexturePKM);
    cxTextureLoad((cxTexture)pkm,stream);
    return (cxTexture)pkm;
}
