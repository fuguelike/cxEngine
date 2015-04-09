//
//  cxTexturePNG.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTexturePNG.h"
#include <engine/cxUtil.h>

CX_METHOD_DEF(cxTexturePNG,Load,cxBool,cxStream stream)
{
    cxBool ret = false;
    CX_ASSERT(stream != NULL, "pvr stream not set");
    cxStr data = cxStreamAllBytes(stream);
    if(data == NULL){
        return false;
    }
    png_image image;
    memset(&image, 0, sizeof(png_image));
    image.version = PNG_IMAGE_VERSION;
    if(!png_image_begin_read_from_memory(&image, cxStrBody(data), cxStrLength(data))){
        CX_ERROR("read png data failed");
        return false;
    }
    image.format = PNG_FORMAT_RGBA;
    this->cxTexture.size = cxSize2fv(image.width, image.height);
    this->cxTexture.hasAlpha = true;
    cxInt bufsiz = PNG_IMAGE_SIZE(image);
    cxAny buffer = allocator->malloc(bufsiz);
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
    CX_METHOD(cxTexturePNG, Load);
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
    CX_CALL(png, Load, CX_M(cxBool,cxStream),stream);
    return png;
}





