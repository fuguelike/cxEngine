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

#define GLSL(x) #x

enum {
    cxVertexAttribPosition,
    cxVertexAttribColor,
    cxVertexAttribTexcoord,
    cxVertexAttribMax,
};

////Attribute names
#define CX_ATTRIBUTE_NAME_COLOR                     "aColor"
#define CX_ATTRIBUTE_NAME_POSITION                  "aPosition"
#define CX_ATTRIBUTE_NAME_TEXCOORD                  "aTexcoord"
#define CX_ATTRIBUTE_NAME_VERTEX                    "aVertex"
//
////Uniform names
#define CX_UNIFORM_MATRIX_MODELVIEW_PROJECT         "cxMatrixModelViewProject"



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

void cxOpenGLDestroy();

void cxOpenGLCheckFeature();

void cxDrawClippingRect(const cxVec2f pos,cxSize2f size);

void cxDrawPoint(const cxVec2f pos,cxColor3f color,cxFloat w);

void cxDrawSolidRect(const cxRect4f rect,const cxColor4f color,cxConstChars skey);

void cxDrawSolidBox(const cxBoxVec3f *box,const cxColor4f color,cxConstChars skey);

void cxDrawLineBox(const cxBoxVec2f *box,const cxColor3f color);

void cxDrawLineLoop(const cxVec2f *vertices,int num,const cxColor3f color);

void cxOpenGLUsingShader(cxConstChars key);

CX_INLINE void cxOpenGLSetBlendFactor(GLenum sfactor, GLenum dfactor)
{
    cxOpenGL this = cxOpenGLInstance();
    if(sfactor == this->blendSrc && dfactor == this->blendDst){
        return;
    }
    if(sfactor == GL_ONE && dfactor == GL_ZERO){
        glDisable(GL_BLEND);
    }else{
        glEnable(GL_BLEND);
        glBlendFunc(sfactor, dfactor);
        this->blendSrc = sfactor;
        this->blendDst = dfactor;
    }
}

CX_INLINE void cxOpenGLSetDepthTest(cxBool on)
{
    if (on){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }else{
        glDisable(GL_DEPTH_TEST);
    }
}

CX_INLINE void cxOpenGLViewport(const cxBox4f box)
{
    glViewport(box.l, box.t, box.r - box.l, box.b - box.t);
}

CX_INLINE void cxOpenGLEnableScissor(const cxRect4f rect)
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(rect.x, rect.y, rect.w, rect.h);
}

CX_INLINE void cxOpenGLSetTexParameter(GLuint type,GLuint value)
{
    glTexParameteri(GL_TEXTURE_2D, type, value);
}

CX_INLINE void cxOpenGLSetTexParameters(const cxTextureParams params)
{
    cxOpenGLSetTexParameter(GL_TEXTURE_MIN_FILTER, params.minFilter);
    cxOpenGLSetTexParameter(GL_TEXTURE_MAG_FILTER, params.magFilter);
    cxOpenGLSetTexParameter(GL_TEXTURE_WRAP_S, params.wrapS);
    cxOpenGLSetTexParameter(GL_TEXTURE_WRAP_T, params.wrapT);
}

CX_INLINE void cxOpenGLGenTextures(GLsizei n,GLuint *textures)
{
    glGenTextures(n, textures);
}

CX_INLINE void cxOpenGLBindTexture(GLuint texture,cxInt idx)
{
    cxOpenGL this = cxOpenGLInstance();
    if (this->activeTextures[idx] != texture){
        this->activeTextures[idx] = texture;
        glActiveTexture(GL_TEXTURE0 + idx);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}

CX_INLINE void cxOpenGLDeleteTexture(GLuint texture,cxInt idx)
{
    cxOpenGL this = cxOpenGLInstance();
    if(this->activeTextures[idx] == texture){
        this->activeTextures[idx] = -1;
    }
    glDeleteTextures(1, &texture);
}

CX_INLINE void cxOpenGLDisableScissor()
{
    glDisable(GL_SCISSOR_TEST);
}

CX_INLINE void cxOpenGLDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    glDrawArrays(mode, first, count);
}

CX_INLINE void cxOpenGLVertexAttribPointer(GLuint indx, GLint size, GLsizei stride, const GLvoid* ptr)
{
    glVertexAttribPointer(indx, size, GL_FLOAT, GL_FALSE, stride, ptr);
}

CX_INLINE void cxOpenGLEnableVertexAttribArray (GLuint index,GLboolean enable)
{
    if(enable){
        glEnableVertexAttribArray(index);
    }else{
        glDisableVertexAttribArray(index);
    }
}

CX_INLINE void cxOpenGLActiveAttribs(cxUInt flags)
{
    cxOpenGL this = cxOpenGLInstance();
    cxBool eposition = (flags & cxVertexAttribFlagPosition) != 0;
    if(eposition != this->enableAttribPosition){
        cxOpenGLEnableVertexAttribArray(cxVertexAttribPosition,eposition);
        this->enableAttribPosition = eposition;
    }
    cxBool ecolor = (flags & cxVertexAttribFlagColor) != 0;
    if(ecolor != this->enableAttribColor){
        cxOpenGLEnableVertexAttribArray(cxVertexAttribColor,ecolor);
        this->enableAttribColor = ecolor;
    }
    cxBool etexcoords = (flags & cxVertexAttribFlagTexcoord);
    if(etexcoords != this->enableAttribTexcoords){
        cxOpenGLEnableVertexAttribArray(cxVertexAttribTexcoord,etexcoords);
        this->enableAttribTexcoords = etexcoords;
    }
}

CX_INLINE void cxOpenGLUseProgram(GLuint program)
{
    cxOpenGL this = cxOpenGLInstance();
    if(this->currentProgram != program){
        glUseProgram(program);
        this->currentProgram = program;
    }
}

CX_INLINE void cxOpenGLDeleteProgram(GLuint program)
{
    cxOpenGL this = cxOpenGLInstance();
    glDeleteProgram(program);
    this->currentProgram = -1;
}

CX_INLINE void cxOpenGLSetClearColor(cxColor4f color)
{
    cxOpenGL this = cxOpenGLInstance();
    this->clearColor = color;
}

CX_INLINE void cxOpenGLClear()
{
    cxOpenGL this = cxOpenGLInstance();
    glClearColor(this->clearColor.r, this->clearColor.g, this->clearColor.b, this->clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


CX_C_END

#endif
