//
//  cxShaderClipping.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"
#include "cxShaderClipping.h"

static cxString CX_METHOD(cxShaderClipping,Fragment)
{
    static cxConstChars fragment =
    GLSL(
        varying mediump vec4 vFragmentColor;
        varying highp vec2 vTexCoord;
        uniform sampler2D uTexture0;
        void main() {
            vec4 texColor = texture2D(uTexture0, vTexCoord);
            if(texColor.a <= 0.0)discard;
            gl_FragColor = vFragmentColor * texColor;
        }
    );
    return cxStringConstChars(fragment);
}

CX_TYPE(cxShaderClipping, cxShader)
{
    CX_MSET(cxShaderClipping, Fragment);
}
CX_INIT(cxShaderClipping, cxShader)
{

}
CX_FREE(cxShaderClipping, cxShader)
{

}
CX_TERM(cxShaderClipping, cxShader)