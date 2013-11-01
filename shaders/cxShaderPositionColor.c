//
//  cxShaderPositionColor.c
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderPositionColor.h"

static void cxShaderPositionColorInit(cxAny this)
{
    cxShaderPositionColor shader = this;
    glBindAttribLocation(shader->super.program, cxVertexAttribPosition, CX_ATTRIBUTE_NAME_POSITION);
    glBindAttribLocation(shader->super.program, cxVertexAttribColor, CX_ATTRIBUTE_NAME_COLOR);
}

static cxString cxShaderPositionColorGetVertexSource(cxAny this)
{
    static cxConstChars vertex =
    "                                                                   \n\
        #ifdef GL_ES                                                    \n\
            attribute highp vec4 aPosition;                             \n\
            attribute lowp vec4 aColor;                                 \n\
            varying lowp vec4 vColor;                                   \n\
        #else                                                           \n\
            attribute vec4 aPosition;                                   \n\
            attribute vec4 aColor;                                      \n\
            varying vec4 vColor;                                        \n\
        #endif                                                          \n\
        void main()                                                     \n\
        {                                                               \n\
            gl_Position = kxMatrixModelViewProject * aPosition;         \n\
            vColor = aColor;                                            \n\
        }                                                               \n\
    ";
    return cxStringConstChars(vertex);
}


static cxString cxShaderPositionColorGetFragmentSource(cxAny this)
{
    static cxConstChars fragment =
    "                                                                   \n\
        #ifdef GL_ES                                                    \n\
            varying lowp vec4 vColor;                                   \n\
        #else                                                           \n\
            varying vec4 vColor;                                        \n\
        #endif                                                          \n\
        void main()                                                     \n\
        {                                                               \n\
            gl_FragColor = vColor;                                      \n\
        }                                                               \n\
    ";
    return cxStringConstChars(fragment);
}

CX_OBJECT_INIT(cxShaderPositionColor, cxShader)
{
    CX_METHOD_SET(this->super.Init, cxShaderPositionColorInit);
    CX_METHOD_SET(this->super.GetVertexSource, cxShaderPositionColorGetVertexSource);
    CX_METHOD_SET(this->super.GetFragmentSource, cxShaderPositionColorGetFragmentSource);
}
CX_OBJECT_FREE(cxShaderPositionColor, cxShader)
{
    
}
CX_OBJECT_TERM(cxShaderPositionColor, cxShader)








