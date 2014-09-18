//
//  cxShaderMultiple.c
//  cxEngine
//
//  Created by xuhua on 9/18/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxShaderMultiple.h"

cxString cxShaderMultipleGetVertexSource(cxAny this)
{
    static cxConstChars vertex =
    GLSL(
         attribute highp vec4 aPosition;
         attribute highp vec2 aTexcoord;
         attribute mediump vec4 aColor;
         varying mediump vec4 vFragmentColor;
         varying highp vec2 vTexCoord;
         void main() {
             gl_Position = cxMatrixModelViewProject * aPosition;
             vFragmentColor = aColor;
             vTexCoord = aTexcoord;
         }
    );
    return cxStringConstChars(vertex);
}

cxString cxShaderMultipleGetFragmentSource(cxAny this)
{
    static cxConstChars fragment =
    GLSL(
         varying mediump vec4 vFragmentColor;
         varying highp vec2 vTexCoord;
         uniform sampler2D uTexture0;
         uniform sampler2D uTexture1;
         void main() {
             vec4 color = texture2D(uTexture1, vTexCoord);
             gl_FragColor = vFragmentColor * texture2D(uTexture0, vTexCoord);
             gl_FragColor.a = color.r;
         }
    );
    return cxStringConstChars(fragment);
}

static void cxShaderMultipleGetUniform(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxShaderMultiple);
    this->texture1 = glGetUniformLocation(this->cxShader.program, "uTexture0");
    if(this->texture1 > 0){
        glUniform1i(this->texture1, 1);
    }
    this->texture2 = glGetUniformLocation(this->cxShader.program, "uTexture1");
    if(this->texture2){
        glUniform1i(this->texture2, 2);
    }
}

CX_OBJECT_TYPE(cxShaderMultiple, cxShader)
{
    
}
CX_OBJECT_INIT(cxShaderMultiple, cxShader)
{
    CX_METHOD_SET(this->cxShader.GetVertexSource, cxShaderMultipleGetVertexSource);
    CX_METHOD_SET(this->cxShader.GetFragmentSource, cxShaderMultipleGetFragmentSource);
    CX_METHOD_SET(this->cxShader.GetUniform, cxShaderMultipleGetUniform);
}
CX_OBJECT_FREE(cxShaderMultiple, cxShader)
{

}
CX_OBJECT_TERM(cxShaderMultiple, cxShader)