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
    GLSL(
        attribute highp vec4 aPosition;
        attribute mediump vec4 aColor;
        varying mediump vec4 vColor;
        void main() {
            gl_Position = kxMatrixModelViewProject * aPosition;
            vColor = aColor;
        }
    );
    return cxStringConstChars(vertex);
}

static cxString cxShaderPositionColorGetFragmentSource(cxAny this)
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

CX_OBJECT_TYPE(cxShaderPositionColor, cxObject)
{
    
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








