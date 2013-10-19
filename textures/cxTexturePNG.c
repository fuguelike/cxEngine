//
//  cxTexturePNG.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTexturePNG.h"

static cxBool cxTexturePNGLoad(cxAny this,cxStream stream)
{
    cxBool ret = false;
    cxTexturePNG png = this;
    CX_ASSERT(stream != NULL, "pvr stream not set");
    cxString data = stream->interface->AllBytes(stream);
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
    png->super.size = cxSize2fv(image.width, image.height);
    png->super.hasAlpha = true;
    char *buffer = allocator->malloc(PNG_IMAGE_SIZE(image));
    if(png_image_finish_read(&image, NULL, buffer, 0, NULL)){
        cxOpenGLGenTextures(1, &png->super.textureId);
        cxOpenGLBindTexture(0, png->super.textureId);
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
    cxTexturePNG pvr = this;
    CX_ASSERT(pvr->super.isLoad, "texture not load");
    cxOpenGLBindTexture(0, pvr->super.textureId);
}

static const cxTextureInterface pngInterface = {
    .Load = cxTexturePNGLoad,
    .Bind = cxTexturePNGBind,
};

CX_OBJECT_INIT(cxTexturePNG, cxTexture)
{
    this->super.interface = &pngInterface;
}
CX_OBJECT_FREE(cxTexturePNG, cxTexture)
{

}
CX_OBJECT_TERM(cxTexturePNG, cxTexture)

cxTexture cxTexturePNGLoadStream(cxStream stream)
{
    cxTexturePNG png = CX_CREATE(cxTexturePNG);
    cxTextureLoad((cxTexture)png,stream);
    return (cxTexture)png;
}





