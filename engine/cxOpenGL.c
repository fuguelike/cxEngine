//
//  cxOpenGL.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include <shaders/cxShaderColor.h>
#include <shaders/cxShaderDefault.h>
#include <shaders/cxShaderAlpha.h>
#include <shaders/cxShaderClipping.h>
#include <shaders/cxShaderGray.h>
#include "cxUtil.h"
#include "cxOpenGL.h"

#define CX_OPENGL_LOAD_SHADER(t)                                    \
do{                                                                 \
    cxShader shader = CX_ALLOC(t);                                  \
    if(cxShaderInit(shader)){                                       \
        cxHashSet(this->shaders, cxHashStrKey(#t"Key"), shader);    \
    }else{                                                          \
        CX_ERROR("shader "#t" init failed");                        \
    }                                                               \
    CX_RELEASE(shader);                                             \
}while(0)

static void cxOpenGLLoadDefaultShaders()
{
    cxOpenGL this = cxOpenGLInstance();
    CX_OPENGL_LOAD_SHADER(cxShaderDefault);
    CX_OPENGL_LOAD_SHADER(cxShaderColor);
    CX_OPENGL_LOAD_SHADER(cxShaderAlpha);
    CX_OPENGL_LOAD_SHADER(cxShaderClipping);
    CX_OPENGL_LOAD_SHADER(cxShaderGray);
}

void cxOpenGLUsingShader(cxConstChars key)
{
    cxOpenGL this = cxOpenGLInstance();
    cxShader shader = cxHashGet(this->shaders, cxHashStrKey(key));
    CX_ASSERT(shader != NULL, "shader %s not exists",key);
    cxShaderUsing(shader,NULL);
}

void cxDrawLineBox(const cxBoxVec2f *box,const cxColor3f color)
{
    cxDrawLineLoop(&box->lt, 4, color);
}

void cxDrawLineLoop(const cxVec2f *vertices,int num,const cxColor3f color)
{
    cxColor3f colors[num];
    for(int i=0; i < num;i++){
        colors[i] = color;
    }
    cxOpenGLSetBlendFactor(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cxOpenGLUsingShader(cxShaderColorKey);
    glEnableVertexAttribArray(cxVertexAttribPosition);
    glVertexAttribPointer(cxVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, sizeof(cxVec2f), vertices);
    glEnableVertexAttribArray(cxVertexAttribColor);
    glVertexAttribPointer(cxVertexAttribColor, 3, GL_FLOAT, GL_FALSE, sizeof(cxColor3f), colors);
    glDrawArrays(GL_LINE_LOOP, 0, num);
}

void cxDrawLines(const cxVec2f *vertices,int num,const cxColor3f color)
{
    cxColor3f colors[num];
    for(int i=0; i < num;i++){
        colors[i] = color;
    }
    cxOpenGLSetBlendFactor(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cxOpenGLUsingShader(cxShaderColorKey);
    glEnableVertexAttribArray(cxVertexAttribPosition);
    glVertexAttribPointer(cxVertexAttribPosition, 2, GL_FLOAT, GL_FALSE, sizeof(cxVec2f), vertices);
    glEnableVertexAttribArray(cxVertexAttribColor);
    glVertexAttribPointer(cxVertexAttribColor, 3, GL_FLOAT, GL_FALSE, sizeof(cxColor3f), colors);
    glDrawArrays(GL_LINES, 0, num);
}

void cxDrawClippingRect(const cxVec2f pos,cxSize2f size)
{
    cxDrawSolidRect(cxRect4fv(pos.x, pos.y, size.w, size.h), cxColor4fv(0, 0, 0, 0), cxShaderDefaultKey);
}

void cxDrawBox(const cxBox4f vbox,cxColor3f color)
{
    cxBoxVec3f box;
    box.lb = cxVec3fv(vbox.l, vbox.b, 0);
    box.lt = cxVec3fv(vbox.l, vbox.t, 0);
    box.rb = cxVec3fv(vbox.r, vbox.b, 0);
    box.rt = cxVec3fv(vbox.r, vbox.t, 0);
    cxDrawSolidBox(&box, cxColor3fToColor4f(color), cxShaderColorKey);
}

void cxDrawPoint(const cxVec2f pos,cxColor3f color,cxFloat w)
{
    GLint lw;
    glGetIntegerv(GL_LINE_WIDTH, &lw);
    glLineWidth(w);
    cxRect4f rect = cxRect4fv(pos.x, pos.y, w, w);
    cxDrawSolidRect(rect, cxColor4fv(color.r, color.g, color.b, 1.0f), cxShaderColorKey);
    glLineWidth(lw);
}

void cxDrawSolidRect(const cxRect4f rect,const cxColor4f color,cxConstChars skey)
{
    cxBoxVec3f box;
    cxFloat wh = rect.w / 2.0f;
    cxFloat hh = rect.h / 2.0f;
    box.lb = cxVec3fv(rect.x - wh, rect.y - hh, 0);
    box.lt = cxVec3fv(rect.x - wh, rect.y + hh, 0);
    box.rb = cxVec3fv(rect.x + wh, rect.y - hh, 0);
    box.rt = cxVec3fv(rect.x + wh, rect.y + hh, 0);
    cxDrawSolidBox(&box, color, skey);
}

void cxDrawSolidBox(const cxBoxVec3f *box,const cxColor4f color,cxConstChars skey)
{
    cxColor4f colors[4] = {color,color,color,color};
    cxOpenGLSetBlendFactor(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cxOpenGLUsingShader(skey);
    glEnableVertexAttribArray(cxVertexAttribPosition);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxVec3f), box);
    glEnableVertexAttribArray(cxVertexAttribColor);
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxColor4f), colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

CX_TYPE(cxOpenGL, cxObject)
{
    
}
CX_INIT(cxOpenGL, cxObject)
{
    this->shaders = CX_ALLOC(cxHash);
    cxOpenGLClear(this);
}
CX_FREE(cxOpenGL, cxObject)
{
    CX_RELEASE(this->shaders);
}
CX_TERM(cxOpenGL, cxObject)

void cxOpenGLClear(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxOpenGL);
    this->blendSrc = -1;
    this->blendDst = -1;
    this->currentProgram = -1;
    for(cxInt i=0; i < MAX_ACTIVE_TEXTURE;i++){
        this->activeTextures[i] = -1;
    }
    cxHashClear(this->shaders);
}

cxAny cxOpenGLShader(cxConstChars key)
{
    CX_ASSERT(key != NULL, "shader key == NULL");
    cxOpenGL this = cxOpenGLInstance();
    return cxHashGet(this->shaders, cxHashStrKey(key));
}

cxAny cxOpenGLShaderByName(cxConstChars name)
{
    if(cxConstCharsEqu(name, "color")){
        return cxOpenGLShader(cxShaderColorKey);
    }
    if(cxConstCharsEqu(name, "clipping")){
        return cxOpenGLShader(cxShaderClippingKey);
    }
    if(cxConstCharsEqu(name, "alpha")){
        return cxOpenGLShader(cxShaderAlphaKey);
    }
    if(cxConstCharsEqu(name, "gray")){
        return cxOpenGLShader(cxShaderGrayKey);
    }
    return cxOpenGLShader(cxShaderDefaultKey);
}

#define CX_GL_SUPPORT(t)                                \
this->support_##t = strstr(extensions,#t) > 0;          \
if(this->support_##t){CX_LOGGER("%s = YES",#t);}        \
else {CX_LOGGER("%s = NO",#t);}

void cxOpenGLCheckFeature()
{
    cxOpenGL this = cxOpenGLInstance();
    
    CX_LOGGER("GL_VENDOR:   %s", glGetString(GL_VENDOR));
    CX_LOGGER("GL_RENDERER: %s", glGetString(GL_RENDERER));
    CX_LOGGER("GL_VERSION:  %s", glGetString(GL_VERSION));
    
    cxConstChars extensions = (cxConstChars)glGetString(GL_EXTENSIONS);
    CX_ASSERT(extensions != NULL, "gl extensions get error");
    
    //GL_OES_mapbuffer
    CX_GL_SUPPORT(GL_IMG_texture_npot);
    CX_GL_SUPPORT(GL_IMG_texture_compression_pvrtc);
    CX_GL_SUPPORT(GL_OES_compressed_ETC1_RGB8_texture);
    CX_GL_SUPPORT(GL_OES_vertex_array_object);
    CX_GL_SUPPORT(GL_EXT_discard_framebuffer);
    CX_GL_SUPPORT(GL_OES_mapbuffer);
    CX_GL_SUPPORT(GL_OES_packed_depth_stencil);
    
    //android use vbo
#if (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID)
    //support GL_OES_vertex_array_object
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES" );
    glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress ("glIsVertexArrayOES");
    this->support_GL_OES_vertex_array_object = false;
    //support GL_OES_mapbuffer
    glMapBuffer = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
    glUnmapBuffer = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
    glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVOESPROC)eglGetProcAddress("glGetBufferPointervOES");
    this->support_GL_OES_mapbuffer = false;
#endif
    
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &this->max_texture_size);
    CX_LOGGER("GL_MAX_TEXTURE_SIZE: %d",this->max_texture_size);
    
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &this->max_combined_texture_units);
    CX_LOGGER("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: %d",this->max_combined_texture_units);
    
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &this->max_vertex_texture_units);
    CX_LOGGER("GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS: %d",this->max_vertex_texture_units);
    
    cxOpenGLLoadDefaultShaders();
}

void OpenGLDrawTriangle(cxInt mode,cxInt num,const cxVec3f *vs,const cxColor4f *cs,const cxTex2f *ts)
{
    CX_RETURN(num < 3);
    if(vs != NULL){
        glEnableVertexAttribArray(cxVertexAttribPosition);
        glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxVec3f), vs);
    }else{
        glDisableVertexAttribArray(cxVertexAttribPosition);
    }
    if(cs != NULL){
        glEnableVertexAttribArray(cxVertexAttribColor);
        glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxColor4f), cs);
    }else{
        glDisableVertexAttribArray(cxVertexAttribColor);
    }
    if(ts != NULL){
        glEnableVertexAttribArray(cxVertexAttribTexcoord);
        glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxTex2f), ts);
    }else{
        glDisableVertexAttribArray(cxVertexAttribTexcoord);
    }
    glDrawArrays(mode, 0, num);
}

void cxOpenGLSetBlendFactor(GLenum sfactor, GLenum dfactor)
{
    cxOpenGL this = cxOpenGLInstance();
    if(sfactor == this->blendSrc && dfactor == this->blendDst){
        return;
    }
    if(sfactor == GL_ONE && dfactor == GL_ZERO){
        glDisable(GL_BLEND);
        return;
    }
    glEnable(GL_BLEND);
    glBlendFunc(sfactor, dfactor);
    this->blendSrc = sfactor;
    this->blendDst = dfactor;
}

void cxOpenGLSetDepthTest(cxBool on)
{
    if (on){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_GEQUAL);
    }else{
        glDisable(GL_DEPTH_TEST);
    }
}

void cxOpenGLViewport(const cxBox4f box)
{
    glViewport(box.l, box.t, box.r - box.l, box.b - box.t);
}

void cxOpenGLEnableScissor(const cxRect4f rect)
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(rect.x, rect.y, rect.w, rect.h);
}

void cxOpenGLSetTexParameter(GLuint type,GLuint value)
{
    glTexParameteri(GL_TEXTURE_2D, type, value);
}

void cxOpenGLSetTexParameters(const cxTextureParams params)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
}

void cxOpenGLGenTextures(GLsizei n,GLuint *textures)
{
    glGenTextures(n, textures);
}

void cxOpenGLBindTexture(GLuint texture,cxInt idx)
{
    cxOpenGL this = cxOpenGLInstance();
    if (this->activeTextures[idx] != texture){
        this->activeTextures[idx] = texture;
        glActiveTexture(GL_TEXTURE0 + idx);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}

void cxOpenGLDeleteTexture(GLuint texture,cxInt idx)
{
    cxOpenGL this = cxOpenGLInstance();
    if(this->activeTextures[idx] == texture){
        this->activeTextures[idx] = -1;
    }
    glDeleteTextures(1, &texture);
}

void cxOpenGLDisableScissor()
{
    glDisable(GL_SCISSOR_TEST);
}

void cxOpenGLUseProgram(GLuint program)
{
    cxOpenGL this = cxOpenGLInstance();
    if(this->currentProgram != program){
        glUseProgram(program);
        this->currentProgram = program;
    }
}

void cxOpenGLDeleteProgram(GLuint program)
{
    cxOpenGL this = cxOpenGLInstance();
    glDeleteProgram(program);
    this->currentProgram = -1;
}

void cxOpenGLSetClearColor(cxColor4f color)
{
    cxOpenGL this = cxOpenGLInstance();
    this->clearColor = color;
}

void cxOpenGLClearColorBuffer()
{
    cxOpenGL this = cxOpenGLInstance();
    glClearColor(this->clearColor.r, this->clearColor.g, this->clearColor.b, this->clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}





