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
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #define glDeleteVertexArrays(n,p)       (void)n;(void)p
    #define glGenVertexArrays(n,p)          (void)n;(void)p
    #define glBindVertexArray(a)            (void)a
#elif (CX_TARGET_PLATFORM == CX_PLATFORM_IOS)
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
    #define glDeleteVertexArrays		glDeleteVertexArraysOES
    #define glGenVertexArrays			glGenVertexArraysOES
    #define glBindVertexArray			glBindVertexArrayOES
#else
    #error "not define target platform"
#endif

CX_C_BEGIN

enum {
    cxVertexAttribFlagPosition    = 1 << 0,
    cxVertexAttribFlagColor       = 1 << 1,
    cxVertexAttribFlagTexcoord    = 1 << 2,
    cxVertexAttribFlagPosColorTex = cxVertexAttribFlagPosition|cxVertexAttribFlagColor|cxVertexAttribFlagTexcoord,
};

//shader key define
CX_STRING_KEY_DEF(cxShaderPositionColorKey);
CX_STRING_KEY_DEF(cxShaderDefaultKey);
CX_STRING_KEY_DEF(cxShaderAlphaKey);
CX_STRING_KEY_DEF(cxShaderClippingKey);
CX_STRING_KEY_DEF(cxShaderMultipleKey);

typedef struct{
    GLuint    minFilter;
    GLuint    magFilter;
    GLuint    wrapS;
    GLuint    wrapT;
}cxTextureParams;

#define cxtpv(n,g,s,t) (cxTextureParams){n,g,s,t}

#define MAX_ACTIVE_TEXTURE 16

CX_OBJECT_DEF(cxOpenGL, cxObject)
    cxInt max_texture_size;
    cxInt max_combined_texture_units;
    cxInt max_vertex_texture_units;
    cxBool support_GL_IMG_texture_npot;
    cxBool support_GL_IMG_texture_compression_pvrtc;
    cxBool support_GL_OES_compressed_ETC1_RGB8_texture;
    cxBool support_GL_OES_vertex_array_object;
    cxBool support_GL_EXT_discard_framebuffer;
    GLuint currentProgram;
    cxHash shaders;
    GLenum blendSrc;
    GLenum blendDst;
    cxBool enableAttribPosition;
    cxBool enableAttribColor;
    cxBool enableAttribTexcoords;
    GLuint activeTextures[MAX_ACTIVE_TEXTURE];
    cxColor4f clearColor;
CX_OBJECT_END(cxOpenGL, cxObject)

cxOpenGL cxOpenGLInstance();

cxAny cxOpenGLShader(cxConstChars key);

cxAny cxOpenGLShaderByName(cxConstChars name);

void cxOpenGLSetClearColor(cxColor4f color);

void cxOpenGLDestroy();

void cxOpenGLCheckFeature();

void cxOpenGLSetDepthTest(cxBool on);

void cxOpenGLViewport(const cxBox4f box);

void cxOpenGLUseProgram(GLuint program);

void cxOpenGLDeleteProgram(GLuint program);

void cxOpenGLUsingShader(cxConstChars key);

void cxOpenGLSetBlendFactor(GLenum sfactor, GLenum dfactor);

void cxOpenGLActiveAttribs(cxUInt flags);

void cxOpenGLEnableScissor(const cxRect4f rect);

void cxOpenGLDisableScissor();

void cxOpenGLClear();

void cxDrawClippingRect(const cxVec2f pos,cxSize2f size);

void cxDrawSolidRect(const cxRect4f rect,const cxColor4f color,cxConstChars skey);

void cxDrawSolidBox(const cxBoxVec3f *box,const cxColor4f color,cxConstChars skey);

void cxDrawLineBox(const cxBoxVec2f *box,const cxColor3f color);

void cxDrawLineLoop(const cxVec2f *vertices,int num,const cxColor3f color);

void cxOpenGLDrawArrays(GLenum mode, GLint first, GLsizei count);

void cxOpenGLVertexAttribPointer(GLuint indx, GLint size, GLsizei stride, const GLvoid *ptr);

void cxOpenGLSetTexParameter(GLuint type,GLuint value);

void cxOpenGLSetTexParameters(const cxTextureParams params);

void cxOpenGLDeleteTexture(GLuint texture,cxInt idx);

void cxOpenGLBindTexture(GLuint texture,cxInt idx);

void cxOpenGLGenTextures(GLsizei n,GLuint *textures);

CX_C_END

#endif
