//
//  cxShaderClipping.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"
#include "cxShaderClipping.h"

static cxString cxShaderClippingGetFragmentSource(cxAny this)
{
    static cxConstChars fragment =
    "                                                                                               \n\
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
        if(texColor.a == 0.0)discard;                                                               \n\
        gl_FragColor = vec4(0.0,0.0,0.0,0.0);                                                       \n\
    }                                                                                               \n\
    ";
    return cxStringConstChars(fragment);
}

CX_OBJECT_INIT(cxShaderClipping, cxShader)
{
    CX_METHOD_SET(this->super.GetVertexSource, cxShaderDefaultGetVertexSource);
    CX_METHOD_SET(this->super.GetFragmentSource, cxShaderClippingGetFragmentSource);
}
CX_OBJECT_FREE(cxShaderClipping, cxShader)
{

}
CX_OBJECT_TERM(cxShaderClipping, cxShader)