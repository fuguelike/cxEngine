//
//  cxShaderColor.c
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderColor.h"

CX_METHOD_DEF(cxShaderColor,Init,void)
{
    glBindAttribLocation(this->cxShader.program, cxVertexAttribPosition, CX_ATTRIBUTE_NAME_POSITION);
    glBindAttribLocation(this->cxShader.program, cxVertexAttribColor, CX_ATTRIBUTE_NAME_COLOR);
}
CX_METHOD_DEF(cxShaderColor,Vertex,cxString)
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
CX_METHOD_DEF(cxShaderColor,Fragment,cxString)
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
    CX_METHOD(cxShaderColor, Fragment);
    CX_METHOD(cxShaderColor, Vertex);
    CX_METHOD(cxShaderColor, Init);
}
CX_INIT(cxShaderColor, cxShader)
{

}
CX_FREE(cxShaderColor, cxShader)
{
    
}
CX_TERM(cxShaderColor, cxShader)








