//
//  cxShaderGray.c
//  cxEngineStatic
//
//  Created by xuhua on 1/28/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include "cxShaderGray.h"

CX_METHOD_DEF(cxShaderGray,Fragment,cxStr)
{
    static cxConstChars fragment = GLSL(
        varying mediump vec4 vFragmentColor;
        varying mediump vec2 vTexCoord;
        uniform sampler2D uTexture0;
        const vec3 grayValue = vec3(0.299, 0.587, 0.114);
        void main() {
            vec4 texColor = texture2D(uTexture0, vTexCoord);
            float vscale = dot(texColor.rgb, grayValue);
            vec4 gray = vec4(vscale, vscale, vscale, texColor.a);
            gl_FragColor = vFragmentColor * mix(texColor, gray, vFragmentColor.a);
        });
    return cxStrConstChars(fragment);
}
CX_TYPE(cxShaderGray, cxShader)
{
    CX_METHOD(cxShaderGray, Fragment);
}
CX_INIT(cxShaderGray, cxShader)
{
    
}
CX_FREE(cxShaderGray, cxShader)
{
    
}
CX_TERM(cxShaderGray, cxShader)