//
//  cxShaderAlpha.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"
#include "cxShaderAlpha.h"

static cxString cxShaderAlphaGetFragmentSource(cxAny this)
{
    static cxConstChars fragment =
    "                                                                                                   \n\
        #ifdef GL_ES                                                                                    \n\
            varying lowp vec4   vFragmentColor;                                                         \n\
            varying lowp vec2   vTexCoord;                                                              \n\
            uniform sampler2D   uTexture0;                                                              \n\
        #else                                                                                           \n\
            varying vec4   vFragmentColor;                                                              \n\
            varying vec2   vTexCoord;                                                                   \n\
            uniform sampler2D   uTexture0;                                                              \n\
        #endif                                                                                          \n\
        void main()                                                                                     \n\
        {                                                                                               \n\
            vec4 texColor = texture2D(uTexture0, vTexCoord);                                            \n\
            gl_FragColor = vec4(vFragmentColor.rgb,vFragmentColor.a * texColor.a);                      \n\
        }                                                                                               \n\
    ";
    return cxStringConstChars(fragment);
}

CX_OBJECT_INIT(cxShaderAlpha, cxShader)
{
    CX_METHOD_SET(this->super.GetVertexSource, cxShaderDefaultGetVertexSource);
    CX_METHOD_SET(this->super.GetFragmentSource, cxShaderAlphaGetFragmentSource);
}
CX_OBJECT_FREE(cxShaderAlpha, cxShader)
{

}
CX_OBJECT_TERM(cxShaderAlpha, cxShader)