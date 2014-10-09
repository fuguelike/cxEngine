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

enum {
    cxVertexAttribFlagPosition    = 1 << 0,
    cxVertexAttribFlagColor       = 1 << 1,
    cxVertexAttribFlagTexcoord    = 1 << 2,
    cxVertexAttribFlagPosColorTex = cxVertexAttribFlagPosition|cxVertexAttribFlagColor|cxVertexAttribFlagTexcoord,
};

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

void cxShaderInitPosColorTex(cxAny pshader);

bool cxShaderInit(cxAny pshader);

void cxShaderUsing(cxAny pshader);

CX_C_END

#endif








