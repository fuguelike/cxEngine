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
    cxBool error;
}cxJPEGError;

static void cxJPGErrorExit(j_common_ptr cinfo)
{
    cxJPEGError *error = (cxJPEGError *)cinfo->err;
    error->error = true;
}

static cxBool CX_METHOD(cxTextureJPG,Load,cxStream stream)
{
    cxBool ret = false;
    struct jpeg_decompress_struct cinfo;
    cxJPEGError error;
    cinfo.err = jpeg_std_error(&error.pub);
    error.error = false;
    error.pub.error_exit = cxJPGErrorExit;
    jpeg_create_decompress(&cinfo);
    if(error.error){
        CX_ERROR("jpg create decompress failed");
        goto finished;
    }
    cxString bytes = CX_CALL(stream, AllBytes, CX_MT(cxString));
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
    this->cxTexture.size = cxSize2fv(cinfo.image_width, cinfo.image_height);
    this->cxTexture.hasAlpha = false;
    jpeg_start_decompress(&cinfo);
    if(error.error){
        CX_ERROR("jpg start decompress failed");
        goto finished;
    }
    int row_stride = cinfo.output_width * cinfo.output_components;
    cxAny data = allocator->malloc(row_stride * cinfo.output_height);
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
    cxOpenGLGenTextures(1, &this->cxTexture.textureId);
    cxOpenGLBindTexture(this->cxTexture.textureId, 0);
    cxOpenGLSetTexParameters(this->cxTexture.texParam);
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

static void CX_METHOD(cxTextureJPG,Bind)
{
    cxOpenGLBindTexture(this->cxTexture.textureId, 0);
}

CX_TYPE(cxTextureJPG, cxTexture)
{
    CX_MSET(cxTextureJPG, Load);
    CX_MSET(cxTextureJPG, Bind);
}
CX_INIT(cxTextureJPG, cxTexture)
{
    
}
CX_FREE(cxTextureJPG, cxTexture)
{
    //
}
CX_TERM(cxTextureJPG, cxTexture)

cxTexture cxTextureJPGLoadStream(cxStream stream)
{
    cxTexture jpg = CX_CREATE(cxTextureJPG);
    CX_CALL(jpg, Load, CX_MT(cxBool,cxStream),stream);
    return jpg;
}












