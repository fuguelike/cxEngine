//
//  cxShaderClipping.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"
#include "cxShaderClipping.h"

const luaL_Reg cxShaderClippingInstanceMethods[] = {
    
    CX_LUA_SUPER(cxShader)
};

const luaL_Reg cxShaderClippingTypeMethods[] = {
    CX_LUA_TYPE(cxShaderClipping)
};

void cxShaderClippingTypeInit()
{
    CX_LUA_LOAD_TYPE(cxShaderClipping);
}

static cxString cxShaderClippingGetFragmentSource(cxAny this)
{
    static cxConstChars fragment =
    GLSL(
        varying mediump vec4 vFragmentColor;
        varying highp vec2 vTexCoord;
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
    CX_METHOD_OVERRIDE(this->super.GetVertexSource, cxShaderDefaultGetVertexSource);
    CX_METHOD_OVERRIDE(this->super.GetFragmentSource, cxShaderClippingGetFragmentSource);
}
CX_OBJECT_FREE(cxShaderClipping, cxShader)
{

}
CX_OBJECT_TERM(cxShaderClipping, cxShader)