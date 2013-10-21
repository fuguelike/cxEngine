//
//  cxShaderDefault.c
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"

cxString cxShaderDefaultGetVertexSource(cxAny this)
{
    static cxConstChars vertex =
    "                                                                                               \n\
    #ifdef GL_ES                                                                                    \n\
        attribute highp vec4 aPosition;                                                             \n\
        attribute lowp vec2 aTexcoord;                                                              \n\
        attribute lowp vec4 aColor;                                                                 \n\
        varying lowp vec4 vFragmentColor;                                                           \n\
        varying lowp vec2 vTexCoord;                                                                \n\
    #else                                                                                           \n\
        attribute vec4 aPosition;                                                                   \n\
        attribute vec2 aTexcoord;                                                                   \n\
        attribute vec4 aColor;                                                                      \n\
        varying vec4 vFragmentColor;                                                                \n\
        varying vec2 vTexCoord;                                                                     \n\
    #endif                                                                                          \n\
    void main()                                                                                     \n\
    {                                                                                               \n\
        gl_Position = kxMatrixModelViewProject * aPosition;                                         \n\
        vFragmentColor = aColor;                                                                    \n\
        vTexCoord = aTexcoord;                                                                      \n\
    }                                                                                               \n\
    ";
    return cxStringConstChars(vertex);
}

cxString cxShaderDefaultGetFragmentSource(cxAny this)
{
    static cxConstChars fragment =
    "                                                                                               \n\
    #ifdef GL_ES                                                                                    \n\
        varying lowp vec4   vFragmentColor;                                                         \n\
        varying lowp vec2   vTexCoord;                                                              \n\
        uniform sampler2D   uTexture0;                                                              \n\
    #else                                                                                           \n\
        varying vec4   vFragmentColor;                                                              \n\
        varying vec2   vTexCoord;                                                                   \n\
        uniform sampler2D   uTexture0;                                                              \n\
    #endif                                                                                          \n\
    void main()                                                                                     \n\
    {                                                                                               \n\
        vec4 texColor = texture2D(uTexture0, vTexCoord);                                            \n\
        gl_FragColor = vFragmentColor * texColor;                                                   \n\
    }                                                                                               \n\
    ";
    return cxStringConstChars(fragment);
}

CX_OBJECT_INIT(cxShaderDefault, cxShader)
{
    CX_METHOD_SET(this->super.GetVertexSource, cxShaderDefaultGetVertexSource);
    CX_METHOD_SET(this->super.GetFragmentSource, cxShaderDefaultGetFragmentSource);
}
CX_OBJECT_FREE(cxShaderDefault, cxShader)
{

}
CX_OBJECT_TERM(cxShaderDefault, cxShader)









