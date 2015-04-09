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

#define GLSL(x) #x

enum {
    cxVertexAttribPosition,
    cxVertexAttribColor,
    cxVertexAttribTexcoord,
    cxVertexAttribMax,
};

//Attribute names
#define CX_ATTRIBUTE_NAME_COLOR                     "aColor"
#define CX_ATTRIBUTE_NAME_POSITION                  "aPosition"
#define CX_ATTRIBUTE_NAME_TEXCOORD                  "aTexcoord"
//Uniform names
#define CX_UNIFORM_MATRIX_MODELVIEW_PROJECT         "cxMatrixModelViewProject"

CX_DEF(cxShader, cxObject)
    cxMatrix4f kxMatrixProject;
    cxMatrix4f kxMatrixModelView;
    cxMatrix4f kxMatrix;
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLint uniformModelViewProject;
CX_END(cxShader, cxObject)

CX_INLINE GLint cxShaderGetUniform(cxAny pthis,cxConstChars name)
{
    CX_ASSERT_THIS(pthis, cxShader);
    GLint idx = glGetUniformLocation(this->program, name);
    CX_ASSERT(idx >= 0, "get uniform location %s failed",name);
    return idx;
}

bool cxShaderInit(cxAny pshader);

void cxShaderUsing(cxAny pshader,cxAny pview);

CX_C_END

#endif








