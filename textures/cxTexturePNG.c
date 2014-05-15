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
    png->super.size = cxSize2fv(image.width, image.height);
    png->super.hasAlpha = true;
    cxPointer buffer = allocator->malloc(PNG_IMAGE_SIZE(image));
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
    cxTexturePNG png = this;
    cxOpenGLBindTexture(0, png->super.textureId);
}

void __cxTexturePNGInitType(cxAny type)
{
    
}

void __cxTexturePNGInitObject(cxAny object,cxAny json,cxAny hash)
{
    
}

CX_OBJECT_INIT(cxTexturePNG, cxTexture)
{
    CX_METHOD_SET(this->super.Bind, cxTexturePNGBind);
    CX_METHOD_SET(this->super.Load, cxTexturePNGLoad);
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





