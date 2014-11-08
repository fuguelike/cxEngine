//
//  cxTexturePNG.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTexturePNG.h"

static cxBool CX_METHOD(cxTexturePNG,Load,cxStream stream)
{
    cxBool ret = false;
    CX_ASSERT(stream != NULL, "pvr stream not set");
    cxString data = CX_CALL(stream, AllBytes, CX_MT(cxString));
    if(data == NULL){
        return false;
    }
    png_image image;
    memset(&image, 0, sizeof(png_image));
    image.version = PNG_IMAGE_VERSION;
    if(!png_image_begin_read_from_memory(&image, cxStringBody(data), cxStringLength(data))){
        CX_ERROR("read png data failed");
        return false;
    }
    image.format = PNG_FORMAT_RGBA;
    this->cxTexture.size = cxSize2fv(image.width, image.height);
    this->cxTexture.hasAlpha = true;
    cxAny buffer = allocator->malloc(PNG_IMAGE_SIZE(image));
    if(png_image_finish_read(&image, NULL, buffer, 0, NULL)){
        cxOpenGLGenTextures(1, &this->cxTexture.textureId);
        cxOpenGLBindTexture(this->cxTexture.textureId, 0);
        cxOpenGLSetTexParameters(this->cxTexture.texParam);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        cxOpenGLBindTexture(0, 0);
        ret = true;
    }
    allocator->free(buffer);
    png_image_free(&image);
    return ret;
}

CX_TYPE(cxTexturePNG, cxTexture)
{
    CX_MSET(cxTexturePNG, Load);
}
CX_INIT(cxTexturePNG, cxTexture)
{
    
}
CX_FREE(cxTexturePNG, cxTexture)
{

}
CX_TERM(cxTexturePNG, cxTexture)

cxTexture cxTexturePNGLoadStream(cxStream stream)
{
    cxTexture png = CX_CREATE(cxTexturePNG);
    CX_CALL(png, Load, CX_MT(cxBool,cxStream),stream);
    return png;
}





