//
//  cxTexturePNG.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTexturePNG.h"

static cxBool cxTexturePNGLoad(cxAny ptex,cxStream stream)
{
    cxBool ret = false;
    CX_ASSERT_THIS(ptex, cxTexturePNG);
    CX_ASSERT(stream != NULL, "pvr stream not set");
    cxString data = cxStreamAllBytes(stream);
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

static void cxTexturePNGBind(cxAny this)
{
    cxTexturePNG png = this;
    cxOpenGLBindTexture(png->cxTexture.textureId, 0);
}

CX_OBJECT_TYPE(cxTexturePNG, cxTexture)
{
    
}
CX_OBJECT_INIT(cxTexturePNG, cxTexture)
{
    CX_SET(cxTexture, this, Bind, cxTexturePNGBind);
    CX_SET(cxTexture, this, Load, cxTexturePNGLoad);
}
CX_OBJECT_FREE(cxTexturePNG, cxTexture)
{

}
CX_OBJECT_TERM(cxTexturePNG, cxTexture)

cxTexture cxTexturePNGLoadStream(cxStream stream)
{
    cxTexture png = CX_CREATE(cxTexturePNG);
    cxTextureLoad(png,stream);
    return png;
}





