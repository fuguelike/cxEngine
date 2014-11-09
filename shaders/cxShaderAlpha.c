//
//  cxShaderAlpha.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"
#include "cxShaderAlpha.h"

CX_METHOD_DEF(cxShaderAlpha,Fragment,cxString)
{
    static cxConstChars fragment =
    GLSL(
        varying mediump vec4 vFragmentColor;
        varying highp vec2 vTexCoord;
        uniform sampler2D uTexture0;
        void main() {
            vec4 texColor = texture2D(uTexture0, vTexCoord);
            gl_FragColor = vec4(vFragmentColor.rgb,vFragmentColor.a * texColor.a);
        }
    );
    return cxStringConstChars(fragment);
}

CX_TYPE(cxShaderAlpha, cxShader)
{
    CX_METHOD(cxShaderAlpha, Fragment);
}
CX_INIT(cxShaderAlpha, cxShader)
{
    
}
CX_FREE(cxShaderAlpha, cxShader)
{

}
CX_TERM(cxShaderAlpha, cxShader)