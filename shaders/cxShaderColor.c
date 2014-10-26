//
//  cxShaderColor.c
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderColor.h"

static void cxShaderColorInit(cxAny this)
{
    cxShaderColor shader = this;
    glBindAttribLocation(shader->cxShader.program, cxVertexAttribPosition, CX_ATTRIBUTE_NAME_POSITION);
    glBindAttribLocation(shader->cxShader.program, cxVertexAttribColor, CX_ATTRIBUTE_NAME_COLOR);
}

static cxString cxShaderColorVertex(cxAny this)
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

static cxString cxShaderColorFragment(cxAny this)
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

CX_TYPE(cxShaderColor, cxShader)
{
    
}
CX_INIT(cxShaderColor, cxShader)
{
    CX_SET(cxShader, this, Init, cxShaderColorInit);
    CX_SET(cxShader, this, Vertex, cxShaderColorVertex);
    CX_SET(cxShader, this, Fragment, cxShaderColorFragment);
}
CX_FREE(cxShaderColor, cxShader)
{
    
}
CX_TERM(cxShaderColor, cxShader)








