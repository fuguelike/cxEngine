//
//  cxShaderAlpha.c
//  cxEngine
//  use:
//  vec4(texColor.rgb / texColor.a,texColor.a)
//  restore Alpha Premultiplied
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"
#include "cxShaderTTF.h"

CX_METHOD_DEF(cxShaderTTF,Fragment,cxString)
{
    static cxConstChars fragment =
    GLSL(
        varying mediump vec4 vFragmentColor;
        varying highp vec2 vTexCoord;
        uniform sampler2D uTexture0;
        void main() {
            vec4 texColor = texture2D(uTexture0, vTexCoord);
            gl_FragColor = vec4(texColor.rgb / texColor.a,texColor.a) * vFragmentColor;
        }
    );
    return cxStringConstChars(fragment);
}

CX_TYPE(cxShaderTTF, cxShader)
{
    CX_METHOD(cxShaderTTF, Fragment);
}
CX_INIT(cxShaderTTF, cxShader)
{
    
}
CX_FREE(cxShaderTTF, cxShader)
{

}
CX_TERM(cxShaderTTF, cxShader)