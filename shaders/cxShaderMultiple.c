//
//  cxShaderMultiple.c
//  cxEngine
//
//  Created by xuhua on 9/18/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxShaderMultiple.h"
#include "cxShaderDefault.h"

cxString cxShaderMultipleFragment(cxAny ps)
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

static void cxShaderMultipleUniform(cxAny ps)
{
    CX_ASSERT_THIS(ps, cxShaderMultiple);
    this->texture1 = glGetUniformLocation(this->cxShader.program, "uTexture1");
    CX_ASSERT(this->texture1 > 0, "uTexture1 not define");
    glUniform1i(this->texture1, 1);
    this->texture2 = glGetUniformLocation(this->cxShader.program, "uTexture2");
    CX_ASSERT(this->texture2 > 0, "uTexture2 not define");
    glUniform1i(this->texture2, 2);
}

CX_TYPE(cxShaderMultiple, cxShader)
{
    
}
CX_INIT(cxShaderMultiple, cxShader)
{
    CX_SET(cxShader, this, Vertex, cxShaderDefaultVertex);
    CX_SET(cxShader, this, Fragment, cxShaderMultipleFragment);
    CX_SET(cxShader, this, Uniform, cxShaderMultipleUniform);
}
CX_FREE(cxShaderMultiple, cxShader)
{

}
CX_TERM(cxShaderMultiple, cxShader)