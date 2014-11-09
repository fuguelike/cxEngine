//
//  cxShaderMultiple.c
//  cxEngine
//
//  Created by xuhua on 9/18/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxShaderMultiple.h"
#include "cxShaderDefault.h"

CX_METHOD_DEF(cxShaderMultiple,Fragment,cxString)
{
    static cxConstChars fragment =
    GLSL(
         varying mediump vec4 vFragmentColor;
         varying highp vec2 vTexCoord;
         uniform sampler2D uTexture1;
         uniform sampler2D uTexture2;
         void main() {
             gl_FragColor = vFragmentColor * texture2D(uTexture1, vTexCoord);
             gl_FragColor.a = texture2D(uTexture2, vTexCoord).r;
         }
    );
    return cxStringConstChars(fragment);
}
CX_METHOD_DEF(cxShaderMultiple,Uniform,void)
{
    this->texture1 = glGetUniformLocation(this->cxShader.program, "uTexture1");
    CX_ASSERT(this->texture1 > 0, "uTexture1 not define");
    glUniform1i(this->texture1, 1);
    this->texture2 = glGetUniformLocation(this->cxShader.program, "uTexture2");
    CX_ASSERT(this->texture2 > 0, "uTexture2 not define");
    glUniform1i(this->texture2, 2);
}
CX_TYPE(cxShaderMultiple, cxShader)
{
    CX_METHOD(cxShaderMultiple, Fragment);
    CX_METHOD(cxShaderMultiple, Uniform);
}
CX_INIT(cxShaderMultiple, cxShader)
{
    
}
CX_FREE(cxShaderMultiple, cxShader)
{

}
CX_TERM(cxShaderMultiple, cxShader)