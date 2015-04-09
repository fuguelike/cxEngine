//
//  cxShader.c
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <kazmath/GL/matrix.h>
#include "cxShader.h"

typedef void (*cxOpenGLInfoFunc)(GLuint program, GLenum pname, GLint *params);

typedef void (*cxOpenGLLogFunc)(GLuint program, GLsizei bufsize, GLsizei *length, GLchar *infolog);

static char* cxShaderLogFunc(GLuint object, cxOpenGLInfoFunc infoFunc, cxOpenGLLogFunc logFunc)
{
    GLint logLength = 0, writeBytes = 0;
    infoFunc(object, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength < 1){
        return NULL;
    }
    char *logBytes = (char*)allocator->malloc(logLength + 1);
    logFunc(object, logLength, &writeBytes, logBytes);
    logBytes[logLength] = '\0';
    return logBytes;
}

static cxStr cxShaderVertexLog(cxAny pshader)
{
    CX_ASSERT_THIS(pshader, cxShader);
    char *log = cxShaderLogFunc(this->vertexShader, (cxOpenGLInfoFunc)&glGetShaderiv, (cxOpenGLLogFunc)&glGetShaderInfoLog);
    return cxStrAttachMem(log, (cxInt)strlen(log));
}

static cxStr cxShaderFragmentLog(cxAny pshader)
{
    CX_ASSERT_THIS(pshader, cxShader);
    char *log = cxShaderLogFunc(this->fragmentShader, (cxOpenGLInfoFunc)&glGetShaderiv, (cxOpenGLLogFunc)&glGetShaderInfoLog);
    return cxStrAttachMem(log, (cxInt)strlen(log));
}

static cxStr cxShaderProgramLog(cxAny pshader)
{
    CX_ASSERT_THIS(pshader, cxShader);
    char *log = cxShaderLogFunc(this->program, (cxOpenGLInfoFunc)&glGetProgramiv, (cxOpenGLLogFunc)&glGetProgramInfoLog);
    return cxStrAttachMem(log, (cxInt)strlen(log));
}

static bool cxShaderCompile(cxAny pshader,GLuint *shader, GLenum type, cxStr source)
{
    CX_ASSERT_THIS(pshader, cxShader);
    GLint status = 0;
    CX_ASSERT(source != NULL,"shader sources NULL");
    *shader = glCreateShader(type);
    if(type == GL_VERTEX_SHADER){
        const GLchar *sources[] = {
            "uniform highp mat4 "CX_UNIFORM_MATRIX_MODELVIEW_PROJECT";\n",
            cxStrBody(source),
        };
        glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    }else if(type == GL_FRAGMENT_SHADER){
        const GLchar *sources[] = {
            "precision mediump float;\n",
            cxStrBody(source),
        };
        glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    }
    glCompileShader(*shader);
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE){
        GLsizei length = 0;
		glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
		GLchar* src = (GLchar *)allocator->malloc(sizeof(GLchar) * length);
		glGetShaderSource(*shader, length, NULL, src);
		CX_ERROR("Failed to compile shader:%s\n", src);
        cxStr log = NULL;
        if (type == GL_VERTEX_SHADER){
            log = cxShaderVertexLog(this);
        }else{
            log = cxShaderFragmentLog(this);
        }
        CX_ERROR("compile shader error:%s",cxStrBody(log));
        allocator->free(src);
        abort();
    }
    return (status == GL_TRUE);
}
CX_METHOD_DEF(cxShader,Init,void)
{
    glBindAttribLocation(this->program, cxVertexAttribPosition, CX_ATTRIBUTE_NAME_POSITION);
    glBindAttribLocation(this->program, cxVertexAttribColor, CX_ATTRIBUTE_NAME_COLOR);
    glBindAttribLocation(this->program, cxVertexAttribTexcoord, CX_ATTRIBUTE_NAME_TEXCOORD);
}
CX_METHOD_DEF(cxShader,Update,void,cxAny pview)
{
    
}
CX_METHOD_DEF(cxShader,Uniform,void)
{
    
}
CX_METHOD_DEF(cxShader, Vertex,cxStr)
{
    static cxConstChars vertex =
    GLSL(
        attribute highp vec3 aPosition;
        attribute mediump vec2 aTexcoord;
        attribute mediump vec4 aColor;
        varying mediump vec4 vFragmentColor;
        varying mediump vec2 vTexCoord;
        void main() {
            gl_Position = cxMatrixModelViewProject * vec4(aPosition,1.0);
            vFragmentColor = aColor;
            vTexCoord = aTexcoord;
     });
    return cxStrConstChars(vertex);
}
CX_METHOD_DEF(cxShader, Fragment,cxStr)
{
    static cxConstChars fragment =
    GLSL(
         varying mediump vec4 vFragmentColor;
         varying mediump vec2 vTexCoord;
         uniform sampler2D uTexture0;
         void main() {
             vec4 color = texture2D(uTexture0, vTexCoord);
             gl_FragColor = vFragmentColor * color;
     });
    return cxStrConstChars(fragment);
}
CX_TYPE(cxShader, cxObject)
{
    CX_METHOD(cxShader, Init);
    CX_METHOD(cxShader, Update);
    CX_METHOD(cxShader, Uniform);
    CX_METHOD(cxShader, Vertex);
    CX_METHOD(cxShader, Fragment);
}
CX_INIT(cxShader, cxObject)
{
    
}
CX_FREE(cxShader, cxObject)
{
    if(this->program != 0){
        cxOpenGLDeleteProgram(this->program);
        this->program = 0;
    }
    if(this->vertexShader != 0){
        glDeleteShader(this->vertexShader);
        this->vertexShader = 0;
    }
    if(this->fragmentShader != 0){
        glDeleteShader(this->fragmentShader);
        this->fragmentShader = 0;
    }
}
CX_TERM(cxShader, cxObject)

void cxShaderUsing(cxAny pshader,cxAny pview)
{
    CX_ASSERT_THIS(pshader, cxShader);
    cxOpenGLUseProgram(this->program);
    kmGLGetMatrix(KM_GL_PROJECTION, &this->kxMatrixProject);
    kmGLGetMatrix(KM_GL_MODELVIEW,  &this->kxMatrixModelView);
    kmMat4Multiply(&this->kxMatrix, &this->kxMatrixProject, &this->kxMatrixModelView);
    glUniformMatrix4fv(this->uniformModelViewProject, 1, GL_FALSE, this->kxMatrix.mat);
    CX_CALL(this, Update, CX_M(void,cxAny),pview);
}

bool cxShaderInit(cxAny pshader)
{
    CX_ASSERT_THIS(pshader, cxShader);
    GLint status = 0;
    cxStr vbytes = CX_CALL(this, Vertex, CX_M(cxStr));
    CX_ASSERT(vbytes != NULL, "get vertex shader source failed");
    cxStr fbytes = CX_CALL(this, Fragment, CX_M(cxStr));
    CX_ASSERT(vbytes != NULL, "get fragment shader source failed");
    if(!cxShaderCompile(this, &this->vertexShader, GL_VERTEX_SHADER, vbytes)){
        CX_ERROR("%s cxShaderCompile GL_VERTEX_SHADER error",CX_NAME_OF(this));
        return false;
    }
    if(!cxShaderCompile(this, &this->fragmentShader, GL_FRAGMENT_SHADER, fbytes)){
        CX_ERROR("%s cxShaderCompile GL_FRAGMENT_SHADER error",CX_NAME_OF(this));
        return false;
    }
    this->program = glCreateProgram();
    if(this->vertexShader){
        glAttachShader(this->program, this->vertexShader);
    }
    if(this->fragmentShader){
        glAttachShader(this->program, this->fragmentShader);
    }
    CX_CALL(this, Init, CX_M(void));
    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE){
        cxStr log = cxShaderProgramLog(this);
        CX_ERROR("link program error:%s",cxStrBody(log));
        return false;
    }
    if(this->vertexShader != 0){
        glDeleteShader(this->vertexShader);
        this->vertexShader = 0;
    }
    if(this->fragmentShader != 0){
        glDeleteShader(this->fragmentShader);
        this->fragmentShader = 0;
    }
    cxOpenGLUseProgram(this->program);
    this->uniformModelViewProject = glGetUniformLocation(this->program, CX_UNIFORM_MATRIX_MODELVIEW_PROJECT);
    CX_CALL(this, Uniform, CX_M(void));
    return true;
}













