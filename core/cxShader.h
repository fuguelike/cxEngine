//
//  cxShader.h
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxShader_h
#define cxEngine_cxShader_h

#include "cxOpenGL.h"
#include "cxBase.h"
#include "cxString.h"

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
#define CX_UNIFORM_MATRIX_MODELVIEW_PROJECT         "kxMatrixModelViewProject"
#define CX_UNIFORM_ATLAS_TEXTURE                    "kxAtlasTexture"

typedef void (*cxShaderFunc)(cxAny shader);

typedef cxString (*cxShaderSource)(cxAny shader);

CX_OBJECT_DEF(cxShader, cxObject)
    cxMatrix4f kxMatrixProject;
    cxMatrix4f kxMatrixModelView;
    cxMatrix4f kxMatrix;
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLint uniformMatrixModelviewProject;
    GLint uniformAtlasTexture;
    CX_METHOD_DEF(cxShaderFunc,     Init);
    CX_METHOD_DEF(cxShaderFunc,     Update);
    CX_METHOD_DEF(cxShaderFunc,     GetUniform);
    CX_METHOD_DEF(cxShaderSource,   GetVertexSource);
    CX_METHOD_DEF(cxShaderSource,   GetFragmentSource);
CX_OBJECT_END(cxShader)

void cxShaderInitPosColorTex(cxAny this);

bool cxShaderInit(cxShader this);

void cxShaderUsing(cxShader this,cxBool isAtlas);

CX_C_END

#endif








