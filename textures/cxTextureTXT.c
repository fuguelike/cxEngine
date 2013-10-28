//
//  cxTextureTXT.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxIconv.h>
#include <core/cxFreeType.h>
#include <core/cxArray.h>
#include "cxTextureTXT.h"

static cxBool cxTextureTXTLoad(cxAny this,cxStream stream)
{
    cxTextureTXT texture = this;
    cxString unicode = cxIconvConvert(texture->string, "UTF-8", "UTF-16LE");
    if(unicode == NULL){
        CX_ERROR("convert to unicode error");
        return false;
    }
    if(texture->fontfile == NULL){
        CX_ERROR("font file not set");
        return false;
    }
    cxFreeFont font = cxFreeTypeCreateFont(cxStringBody(texture->fontfile));
    if(font == NULL){
        CX_ERROR("open font file error");
        return false;
    }
    cxInt fontsize = floorf(texture->attr.size);
    cxFreeFontSetSize(font, 0, fontsize);
    cxInt des = font->face->size->metrics.descender >> 6;
    cxInt height = font->face->size->metrics.height >> 6;
    cxInt width = 0;
    cxArray list = CX_CREATE(cxArray);
    //get texture width and height
    cxFreeFontChar slast = NULL;
    cxFreeFontChar sfirst = NULL;
    cxInt wx = 0;
    for(int i=0; i < cxStringLength(unicode);i += 2){
        uint16_t code = *(uint16_t *)&unicode->strptr.d[i];
        cxFreeFontCharKey key = cxFontChar(code, fontsize);
        cxFreeFontChar pchar = cxFreeFontLoadChar(font, key);
        if(pchar == NULL){
            continue;
        }
        if(sfirst == NULL){
            sfirst = pchar;
        }
        cxArrayAppend(list, pchar);
        width += (pchar->ax >> 16);
        slast = pchar;
        width = pchar->width + wx + pchar->left;
        wx += (pchar->ax >> 16);
    }
    cxInt x = 0;
    cxInt y = 0;
    if(sfirst != NULL && sfirst->left < 0){
        width -= sfirst->left;
        x -= sfirst->left;
    }
    if(slast != NULL && slast->left > 0){
        width += slast->left;
    }
    //merage text
    cxInt bytes = width * height;
    cxChar *buffer = allocator->calloc(1,bytes);
    CX_ARRAY_FOREACH(list, element){
        cxFreeFontChar pchar = cxArrayObject(element);
        y = height - pchar->top + des;
        for(int i=0;i<pchar->height;i++)
        for(int j=0;j<pchar->width;j++){
            cxChar *src = pchar->data + i * pchar->width + j;
            cxChar *dst = buffer + (i + y) * width + x + pchar->left + j;
            if(*src != 0)*dst = *src;
        }
        x += (pchar->ax >> 16);
    }
    texture->super.hasAlpha = true;
    texture->super.size = cxSize2fv(width, height);
    GLint unpack = 0;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
    cxOpenGLGenTextures(1, &texture->super.textureId);
    cxOpenGLBindTexture(0, texture->super.textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, buffer);
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
    cxOpenGLBindTexture(0, 0);
    allocator->free(buffer);
    return true;
}

static void cxTextureTXTBind(cxAny this)
{
    cxTextureTXT txt = this;
    cxOpenGLBindTexture(0, txt->super.textureId);
}

static const cxTextureInterface txtInterface = {
    .Load = cxTextureTXTLoad,
    .Bind = cxTextureTXTBind,
};

CX_OBJECT_INIT(cxTextureTXT, cxTexture)
{
    this->super.interface = &txtInterface;
}
CX_OBJECT_FREE(cxTextureTXT, cxTexture)
{
    CX_RELEASE(this->string);
    CX_RELEASE(this->fontfile);
}
CX_OBJECT_TERM(cxTextureTXT, cxTexture)


