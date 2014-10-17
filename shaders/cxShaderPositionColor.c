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
    glBindAttribLocation(shader->cxShader.program, cxVertexAttribPosition, CX_ATTRIBUTE_NAME_POSITION);
    glBindAttribLocation(shader->cxShader.program, cxVertexAttribColor, CX_ATTRIBUTE_NAME_COLOR);
}

static cxString cxShaderPositionColorVertex(cxAny this)
{
    static cxConstChars vertex =
    GLSL(
        attribute highp vec4 aPosition;
        attribute mediump vec4 aColor;
        varying mediump vec4 vColor;
        void main() {
            gl_Position = cxMatrixModelViewProject * aPosition;
            vColor = aColor;
        }
    );
    return cxStringConstChars(vertex);
}

static cxString cxShaderPositionColorFragment(cxAny this)
{
    static cxConstChars fragment =
    GLSL(
        varying mediump vec4 vColor;
        void main() {
            gl_FragColor = vColor;
        }
    );
    return cxStringConstChars(fragment);
}

CX_TYPE(cxShaderPositionColor, cxShader)
{
    
}
CX_INIT(cxShaderPositionColor, cxShader)
{
    CX_SET(cxShader, this, Init, cxShaderPositionColorInit);
    CX_SET(cxShader, this, Vertex, cxShaderPositionColorVertex);
    CX_SET(cxShader, this, Fragment, cxShaderPositionColorFragment);
}
CX_FREE(cxShaderPositionColor, cxShader)
{
    
}
CX_TERM(cxShaderPositionColor, cxShader)








