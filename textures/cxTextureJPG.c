//
//  cxTextureJPG.c
//  cxEngineIOS
//
//  Created by xuhua on 11/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTextureJPG.h"
#include <libjpeg/jpeglib.h>

typedef struct {
    struct jpeg_error_mgr pub;
    int error;
}cxJPEGError;

static void cxJPGErrorExit(j_common_ptr cinfo)
{
    cxJPEGError *error = (cxJPEGError *)cinfo->err;
    error->error = 1;
}

static cxBool cxTextureJPGLoad(cxAny this,cxStream stream)
{
    cxBool ret = false;
    cxTextureJPG jpg = this;
    struct jpeg_decompress_struct cinfo;
    cxJPEGError error;
    cinfo.err = jpeg_std_error(&error.pub);
    error.error = 0;
    error.pub.error_exit = cxJPGErrorExit;
    jpeg_create_decompress(&cinfo);
    if(error.error){
        CX_ERROR("jpg create decompress failed");
        goto finished;
    }
    cxString bytes = cxStreamAllBytes(stream);
    if(bytes == NULL){
        CX_ERROR("jpg read stream data error");
        goto finished;
    }
    jpeg_mem_src(&cinfo, (cxUChar *)cxStringBody(bytes), cxStringLength(bytes));
    jpeg_read_header(&cinfo, true);
    if(error.error){
        CX_ERROR("jpg read head failed");
        goto finished;
    }
    jpg->super.size = cxSize2fv(cinfo.image_width, cinfo.image_height);
    jpg->super.hasAlpha = false;
    jpeg_start_decompress(&cinfo);
    if(error.error){
        CX_ERROR("jpg start decompress failed");
        goto finished;
    }
    int row_stride = cinfo.output_width * cinfo.output_components;
    cxPointer data = allocator->malloc(row_stride * cinfo.output_height);
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    int line = 0;
    while(cinfo.output_scanline < cinfo.output_height){
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(data + line * row_stride, buffer[0], row_stride);
        line++;
    }
    GLint unpack = 0;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    cxOpenGLGenTextures(1, &jpg->super.textureId);
    cxOpenGLBindTexture(0, jpg->super.textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cinfo.image_width, cinfo.image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
    cxOpenGLBindTexture(0, 0);
    allocator->free(data);
    ret = true;
finished:
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    return ret;
}

static void cxTextureJPGBind(cxAny this)
{
    cxTextureJPG png = this;
    cxOpenGLBindTexture(0, png->super.textureId);
}

CX_OBJECT_INIT(cxTextureJPG, cxTexture)
{
    CX_METHOD_SET(this->super.Bind, cxTextureJPGBind);
    CX_METHOD_SET(this->super.Load, cxTextureJPGLoad);
}
CX_OBJECT_FREE(cxTextureJPG, cxTexture)
{
    //
}
CX_OBJECT_TERM(cxTextureJPG, cxTexture)

cxTexture cxTextureJPGLoadStream(cxStream stream)
{
    cxTexture jpg = CX_CREATE(cxTextureJPG);
    cxTextureLoad(jpg,stream);
    return jpg;
}












