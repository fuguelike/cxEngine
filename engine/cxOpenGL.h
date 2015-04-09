//
//  cxOpenGL.h
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxOpenGL_h
#define cxEngine_cxOpenGL_h

#include <cxcore/cxBase.h>

#if (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID)
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    PFNGLGENVERTEXARRAYSOESPROC         glGenVertexArrays;
    PFNGLBINDVERTEXARRAYOESPROC         glBindVertexArray;
    PFNGLDELETEVERTEXARRAYSOESPROC      glDeleteVertexArrays;
    PFNGLISVERTEXARRAYOESPROC           glIsVertexArray;
    PFNGLMAPBUFFEROESPROC               glMapBuffer;
    PFNGLUNMAPBUFFEROESPROC             glUnmapBuffer;
    PFNGLGETBUFFERPOINTERVOESPROC       glGetBufferPointerv;
    #define GL_DEPTH24_STENCIL8			GL_DEPTH24_STENCIL8_OES
    #define GL_WRITE_ONLY				GL_WRITE_ONLY_OES
#elif (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
    #define glDeleteVertexArrays		glDeleteVertexArraysOES
    #define glGenVertexArrays			glGenVertexArraysOES
    #define glBindVertexArray			glBindVertexArrayOES
    #define glIsVertexArray             glIsVertexArrayOES
    #define glMapBuffer                 glMapBufferOES
    #define glUnmapBuffer               glUnmapBufferOES
    #define glGetBufferPointerv         glGetBufferPointervOES
    #define GL_DEPTH24_STENCIL8			GL_DEPTH24_STENCIL8_OES
    #define GL_WRITE_ONLY				GL_WRITE_ONLY_OES
#else
    #error "not define CX_TARGET_PLATFORM"
#endif

CX_C_BEGIN

//shader key define
CX_STRING_KEY_DEF(cxShaderColorKey);
CX_STRING_KEY_DEF(cxShaderAlphaKey);
CX_STRING_KEY_DEF(cxShaderClippingKey);
CX_STRING_KEY_DEF(cxShaderDefaultKey);
CX_STRING_KEY_DEF(cxShaderGrayKey);

typedef struct {
    GLushort indices[6];
}cxIndices;

typedef struct{
    GLuint minFilter;
    GLuint magFilter;
    GLuint wrapS;
    GLuint wrapT;
}cxTextureParams;

#define cxtpv(n,g,s,t) (cxTextureParams){n,g,s,t}

#define MAX_ACTIVE_TEXTURE (GL_TEXTURE31 - GL_TEXTURE0 + 1)

CX_DEF(cxOpenGL, cxObject)
    cxInt max_texture_size;
    cxInt max_combined_texture_units;
    cxInt max_vertex_texture_units;
    cxBool support_GL_IMG_texture_npot;
    cxBool support_GL_IMG_texture_compression_pvrtc;
    cxBool support_GL_OES_compressed_ETC1_RGB8_texture;
    cxBool support_GL_OES_vertex_array_object;
    cxBool support_GL_EXT_discard_framebuffer;
    cxBool support_GL_OES_mapbuffer;
    cxBool support_GL_OES_packed_depth_stencil;
    cxBool support_GLES3;
    GLuint currentProgram;
    cxHash shaders;
    GLenum blendSrc;
    GLenum blendDst;
    GLuint activeTextures[MAX_ACTIVE_TEXTURE];
    cxColor4f clearColor;
CX_END(cxOpenGL, cxObject)

void cxOpenGLClear(cxAny pthis);

cxAny cxOpenGLShader(cxConstChars key);

cxAny cxOpenGLShaderByName(cxConstChars name);

void cxOpenGLCheckFeature();

void OpenGLDrawTriangle(cxInt mode,cxInt num,const cxVec3f *vs,const cxColor4f *cs,const cxTex2f *ts);

void cxDrawClippingRect(const cxVec2f pos,cxSize2f size);

void cxDrawBox(const cxBox4f vbox,cxColor3f color);

void cxDrawPoint(const cxVec2f pos,cxColor3f color,cxFloat w);

void cxDrawSolidRect(const cxRect4f rect,const cxColor4f color,cxConstChars skey);

void cxDrawSolidBox(const cxBoxVec3f *box,const cxColor4f color,cxConstChars skey);

void cxDrawLineBox(const cxBoxVec2f *box,const cxColor3f color);

void cxDrawLineLoop(const cxVec2f *vertices,int num,const cxColor3f color);

void cxDrawLines(const cxVec2f *vertices,int num,const cxColor3f color);

void cxOpenGLUsingShader(cxConstChars key);

void cxOpenGLSetBlendFactor(GLenum sfactor, GLenum dfactor);

void cxOpenGLSetDepthTest(cxBool on);

void cxOpenGLViewport(const cxBox4f box);

void cxOpenGLEnableScissor(const cxRect4f rect);

void cxOpenGLSetTexParameter(GLuint type,GLuint value);

void cxOpenGLSetTexParameters(const cxTextureParams params);

void cxOpenGLGenTextures(GLsizei n,GLuint *textures);

void cxOpenGLBindTexture(GLuint texture,cxInt idx);

void cxOpenGLDeleteTexture(GLuint texture,cxInt idx);

void cxOpenGLDisableScissor();

void cxOpenGLUseProgram(GLuint program);

void cxOpenGLDeleteProgram(GLuint program);

void cxOpenGLSetClearColor(cxColor4f color);

void cxOpenGLClearColorBuffer();

CX_C_END

#endif
