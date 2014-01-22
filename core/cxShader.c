//
//  cxShader.c
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <kazmath/matrix.h>
#include "cxString.h"
#include "cxShader.h"

static cxInt cxShaderLuaUsing(lua_State *L)
{
    cxInt top = lua_gettop(L);
    CX_LUA_DEF_THIS(cxShader);
    cxBool atlas = top >= 2 ? lua_toboolean(L, 2) : false;
    cxShaderUsing(this, atlas);
    return 0;
}

CX_LUA_METHOD_BEG(cxShader)
    {"Using",cxShaderLuaUsing},
CX_LUA_METHOD_END(cxShader)

void __cxShaderTypeInit()
{
    CX_LUA_LOAD_TYPE(cxShader);
}

void cxShaderInitPosColorTex(cxAny this)
{
    cxShader shader = this;
    glBindAttribLocation(shader->program, cxVertexAttribPosition, CX_ATTRIBUTE_NAME_POSITION);
    glBindAttribLocation(shader->program, cxVertexAttribColor, CX_ATTRIBUTE_NAME_COLOR);
    glBindAttribLocation(shader->program, cxVertexAttribTexcoord, CX_ATTRIBUTE_NAME_TEXCOORD);
}

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

static cxString cxShaderVertexLog(cxShader this)
{
    char *log = cxShaderLogFunc(this->vertexShader, (cxOpenGLInfoFunc)&glGetShaderiv, (cxOpenGLLogFunc)&glGetShaderInfoLog);
    return cxStringAttach(log, (cxInt)strlen(log));
}

static cxString cxShaderFragmentLog(cxShader this)
{
    char *log = cxShaderLogFunc(this->fragmentShader, (cxOpenGLInfoFunc)&glGetShaderiv, (cxOpenGLLogFunc)&glGetShaderInfoLog);
    return cxStringAttach(log, (cxInt)strlen(log));
}

static cxString cxShaderProgramLog(cxShader this)
{
    char *log = cxShaderLogFunc(this->program, (cxOpenGLInfoFunc)&glGetProgramiv, (cxOpenGLLogFunc)&glGetProgramInfoLog);
    return cxStringAttach(log, (cxInt)strlen(log));
}

static bool cxShaderCompile(cxShader this,GLuint *shader, GLenum type, cxString source)
{
    GLint status = 0;
    CX_ASSERT(source != NULL,"shader sources NULL");
    *shader = glCreateShader(type);
    if(type == GL_VERTEX_SHADER){
        const GLchar *sources[] = {
            "precision mediump float;\n",
            "uniform highp mat4 "CX_UNIFORM_MATRIX_MODELVIEW_PROJECT";\n",
            cxStringBody(source),
        };
        glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    }else if(type == GL_FRAGMENT_SHADER){
        const GLchar *sources[] = {
            "precision mediump float;\n",
            "uniform bool "CX_UNIFORM_ATLAS_TEXTURE";\n",
            cxStringBody(source),
        };
        glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    }else {
        CX_ERROR("type error");
        return false;
    }
    glCompileShader(*shader);
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE){
        GLsizei length = 0;
		glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
		GLchar* src = (GLchar *)allocator->malloc(sizeof(GLchar) * length);
		glGetShaderSource(*shader, length, NULL, src);
		CX_ERROR("Failed to compile shader:\n%s", src);
        cxString log = NULL;
        if (type == GL_VERTEX_SHADER){
            log = cxShaderVertexLog(this);
        }else{
            log = cxShaderFragmentLog(this);
        }
        if(log != NULL){
            CX_ERROR("compile shader error:%s",cxStringBody(log));
        }
        allocator->free(src);
        abort();
    }
    return (status == GL_TRUE);
}

CX_OBJECT_INIT(cxShader, cxObject)
{
    this->super.cxBase = cxBaseTypeShader;
    CX_METHOD_OVERRIDE(this->Init, cxShaderInitPosColorTex);
}
CX_OBJECT_FREE(cxShader, cxObject)
{
    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);
    cxOpenGLDeleteProgram(this->program);
    CX_METHOD_RELEASE(this->Init);
    CX_METHOD_RELEASE(this->Update);
    CX_METHOD_RELEASE(this->GetUniform);
    CX_METHOD_RELEASE(this->GetVertexSource);
    CX_METHOD_RELEASE(this->GetFragmentSource);
}
CX_OBJECT_TERM(cxShader, cxObject)

void cxShaderUsing(cxShader this,cxBool isAtlas)
{
    cxOpenGLUseProgram(this->program);
	kmGLGetMatrix(KM_GL_PROJECTION, &this->kxMatrixProject);
	kmGLGetMatrix(KM_GL_MODELVIEW,  &this->kxMatrixModelView);
    kmMat4Multiply(&this->kxMatrix, &this->kxMatrixProject, &this->kxMatrixModelView);
    glUniformMatrix4fv(this->uniformMatrixModelviewProject, 1, GL_FALSE, this->kxMatrix.mat);
    glUniform1i(this->uniformAtlasTexture, isAtlas);
    CX_METHOD_FIRE(NULL, this->Update, this);
}

bool cxShaderInit(cxShader this)
{
    cxString vbytes = CX_METHOD_FIRE(NULL, this->GetVertexSource, this);
    CX_ASSERT(vbytes != NULL, "get vertex shader source failed");
    cxString fbytes = CX_METHOD_FIRE(NULL, this->GetFragmentSource, this);
    CX_ASSERT(vbytes != NULL, "get fragment shader source failed");
    if(!cxShaderCompile(this, &this->vertexShader, GL_VERTEX_SHADER, vbytes)){
        return false;
    }
    if(!cxShaderCompile(this, &this->fragmentShader, GL_FRAGMENT_SHADER, fbytes)){
        return false;
    }
    this->program = glCreateProgram();
    if(this->vertexShader){
        glAttachShader(this->program, this->vertexShader);
    }
    if(this->fragmentShader){
        glAttachShader(this->program, this->fragmentShader);
    }
    CX_METHOD_FIRE(NULL, this->Init, this);
    GLint status = 0;
    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE){
        cxString log = cxShaderProgramLog(this);
        if(log != NULL){
            CX_ERROR("link program error:%s",cxStringBody(log));
        }
        return false;
    }
    cxOpenGLUseProgram(this->program);
    this->uniformMatrixModelviewProject = glGetUniformLocation(this->program, CX_UNIFORM_MATRIX_MODELVIEW_PROJECT);
    this->uniformAtlasTexture = glGetUniformLocation(this->program, CX_UNIFORM_ATLAS_TEXTURE);
    CX_METHOD_FIRE(NULL, this->GetUniform, this);
    return true;
}













