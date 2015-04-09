//
//  cxTextureXTF.c
//  cxEngineStatic
//
//  Created by xuhua on 11/29/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <lzma/cxLzma.h>
#include "cxTextureXTF.h"

CX_METHOD_DEF(cxTextureXTF,Load,cxBool,cxStream stream)
{
    CX_ASSERT(stream != NULL, "pvr stream not set");
    if(!cxStreamOpen(stream)){
        CX_ERROR("open atf stream failed");
        return false;
    }
    cxTextureXTFHeader header;
    if(cxStreamRead(stream, &header, sizeof(cxTextureXTFHeader)) != sizeof(cxTextureXTFHeader)){
        return false;
    }
    if(header.flags != 0x81052901){
        CX_ERROR("atf file flags error");
        return false;
    }
    cxTextureXTFBlock blocks[header.rgbaNum];
    if(cxStreamRead(stream, &blocks, sizeof(cxTextureXTFBlock)*header.rgbaNum) != sizeof(cxTextureXTFBlock) *header.rgbaNum){
        return false;
    }
    //read json
    cxAny jsonBuf = allocator->malloc(header.jsonLen);
    if(cxStreamRead(stream, jsonBuf, header.jsonLen) != header.jsonLen){
        allocator->free(jsonBuf);
        return false;
    }
    //read texture index
    cxStr jsonzip = cxStrAttachMem(jsonBuf, header.jsonLen);
    cxStr jsondata = cxLzmaDecompress(jsonzip);
    if(!cxStrOK(jsondata)){
        CX_ERROR("lzma uncompress error,set lzma flags");
        return false;
    }
    cxJson json = cxJsonCreate(jsondata);
    if(json == NULL){
        CX_ERROR("create json error");
        return false;
    }
    cxJson meta = cxJsonObject(json, "meta");
    if(meta == NULL){
        CX_ERROR("read json meta object error");
        return false;
    }
    this->cxTexture.scale = cxJsonDouble(meta, "scale", this->cxTexture.scale);
    //parse frames
    cxJson frames = cxJsonArray(json, "frames");
    CX_JSON_ARRAY_EACH_BEG(frames, item)
    cxTexCoord e = CX_ALLOC(cxTexCoord);
    cxConstChars sn = cxJsonConstChars(item, "filename");
    CX_ASSERT(sn != NULL, "filename node miss");
    e->rotated = cxJsonBool(item, "rotated", false);
    e->trimmed = cxJsonBool(item, "trimmed", false);
    e->frame = cxJsonRect4f(item, "frame",cxRect4fv(0, 0, 0, 0));
    e->sourceSize = cxJsonSize2f(item, "sourceSize", cxSize2fv(0, 0));
    e->spriteSourceSize = cxJsonRect4f(item, "spriteSourceSize",cxRect4fv(0, 0, 0, 0));
    cxHashSet(this->cxTexture.keys, cxHashStrKey(sn), e);
    CX_RELEASE(e);
    CX_JSON_ARRAY_EACH_END(frames, item);
    //read texture data
    cxTextureXTFBlock block = blocks[0];
    cxAny rgbaBuf = allocator->malloc(block.rgbaLen);
    if(cxStreamRead(stream, rgbaBuf, block.rgbaLen) != block.rgbaLen){
        CX_ERROR("atf read block error");
        allocator->free(rgbaBuf);
        return false;
    }
    cxStr rgbazip = cxStrAttachMem(rgbaBuf, block.rgbaLen);
    cxStr rgbaData = cxLzmaDecompress(rgbazip);
    if(!cxStrOK(rgbaData)){
        CX_ERROR("read rgba data error");
        return false;
    }
    this->cxTexture.size = cxSize2fv(block.width, block.height);
    this->cxTexture.hasAlpha = true;
    cxOpenGLGenTextures(1, &this->cxTexture.textureId);
    cxOpenGLBindTexture(this->cxTexture.textureId, 0);
    cxOpenGLSetTexParameters(this->cxTexture.texParam);
    cxAny buffer = cxStrBody(rgbaData);
    if(block.format == cxTextureXTFBlockFormat_RGBA8888){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, block.width, block.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    }else if(block.format == cxTextureXTFBlockFormat_RGB888){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, block.width, block.height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    }else if(block.format == cxTextureXTFBlockFormat_ALPHA){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, block.width, block.height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, buffer);
    }else{
        CX_ASSERT_FALSE("rgba data format error");
    }
    cxOpenGLBindTexture(0, 0);
    cxStreamClose(stream);
    return true;
}
CX_TYPE(cxTextureXTF, cxTexture)
{
    CX_METHOD(cxTextureXTF, Load);
}
CX_INIT(cxTextureXTF, cxTexture)
{
    this->textures = CX_ALLOC(cxArray);
}
CX_FREE(cxTextureXTF, cxTexture)
{
    CX_RELEASE(this->textures);
}
CX_TERM(cxTextureXTF, cxTexture)

cxTexture cxTextureXTFLoadStream(cxStream stream)
{
    cxTexture this = CX_CREATE(cxTextureXTF);
    CX_CALL(this, Load, CX_M(cxBool,cxStream),stream);
    return this;
}

