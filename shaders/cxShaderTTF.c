//
//  cxShaderAlpha.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"
#include "cxShaderTTF.h"

static cxString cxShaderTTFFragment(cxAny ps)
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

CX_TYPE(cxShaderTTF, cxShader)
{
    
}
CX_INIT(cxShaderTTF, cxShader)
{
    CX_SET(cxShader, this, Vertex, cxShaderDefaultVertex);
    CX_SET(cxShader, this, Fragment, cxShaderTTFFragment);
}
CX_FREE(cxShaderTTF, cxShader)
{

}
CX_TERM(cxShaderTTF, cxShader)