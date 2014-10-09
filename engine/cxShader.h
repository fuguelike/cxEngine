//
//  cxShader.h
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxShader_h
#define cxEngine_cxShader_h

#include <cxcore/cxBase.h>
#include "cxOpenGL.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxShader, cxObject)
    cxMatrix4f kxMatrixProject;
    cxMatrix4f kxMatrixModelView;
    cxMatrix4f kxMatrix;
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLint uniformModelViewProject;
    CX_METHOD_DEF(void,Init,cxAny);
    CX_METHOD_DEF(void,Update,cxAny);
    CX_METHOD_DEF(void,Uniform,cxAny);
    CX_METHOD_DEF(cxString, Vertex, cxAny);
    CX_METHOD_DEF(cxString, Fragment, cxAny);
CX_OBJECT_END(cxShader, cxObject)

CX_INLINE void cxShaderInitPosColorTex(cxAny pshader)
{
    CX_ASSERT_THIS(pshader, cxShader);
    glBindAttribLocation(this->program, cxVertexAttribPosition, CX_ATTRIBUTE_NAME_POSITION);
    glBindAttribLocation(this->program, cxVertexAttribColor, CX_ATTRIBUTE_NAME_COLOR);
    glBindAttribLocation(this->program, cxVertexAttribTexcoord, CX_ATTRIBUTE_NAME_TEXCOORD);
}

bool cxShaderInit(cxAny pshader);

CX_INLINE void cxShaderUsing(cxAny pshader)
{
    CX_ASSERT_THIS(pshader, cxShader);
    cxOpenGLUseProgram(this->program);
    kmGLGetMatrix(KM_GL_PROJECTION, &this->kxMatrixProject);
    kmGLGetMatrix(KM_GL_MODELVIEW,  &this->kxMatrixModelView);
    kmMat4Multiply(&this->kxMatrix, &this->kxMatrixProject, &this->kxMatrixModelView);
    glUniformMatrix4fv(this->uniformModelViewProject, 1, GL_FALSE, this->kxMatrix.mat);
    CX_METHOD_RUN(this->Update, this);
}

CX_C_END

#endif








