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
    GLSL
    (
        varying lowp vec4 vFragmentColor;
        varying lowp vec2 vTexCoord;
        uniform sampler2D uTexture0;
        void main() {
            vec4 texColor = texture2D(uTexture0, vTexCoord);
            if(kxAtlasTexture)texColor.a=texture2D(uTexture0,vec2(vTexCoord.x,vTexCoord.y+0.5)).r;
            if(texColor.a <= 0.0)discard;
            gl_FragColor = vFragmentColor * texColor;
        }
    );
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