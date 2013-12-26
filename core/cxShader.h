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

CX_OBJECT_DEF(cxShader, cxObject)
    cxMatrix4f kxMatrixProject;
    cxMatrix4f kxMatrixModelView;
    cxMatrix4f kxMatrix;
    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLint uniformMatrixModelviewProject;
    GLint uniformAtlasTexture;
    CX_METHOD_ALLOC(void,Init,cxAny);
    CX_METHOD_ALLOC(void,Update,cxAny);
    CX_METHOD_ALLOC(void,GetUniform,cxAny);
    CX_METHOD_ALLOC(cxString, GetVertexSource, cxAny);
    CX_METHOD_ALLOC(cxString, GetFragmentSource, cxAny);
CX_OBJECT_END(cxShader)

void cxShaderInitPosColorTex(cxAny this);

bool cxShaderInit(cxShader this);

void cxShaderUsing(cxShader this,cxBool isAtlas);

CX_C_END

#endif








