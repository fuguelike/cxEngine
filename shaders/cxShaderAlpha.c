//
//  cxShaderAlpha.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"
#include "cxShaderAlpha.h"

const luaL_Reg cxShaderAlphaInstanceMethods[] = {
    
    CX_LUA_SUPER(cxShader)
};

const luaL_Reg cxShaderAlphaTypeMethods[] = {
    CX_LUA_TYPE(cxShaderAlpha)
};

void cxShaderAlphaTypeInit()
{
    CX_LUA_LOAD_TYPE(cxShaderAlpha);
}

static cxString cxShaderAlphaGetFragmentSource(cxAny this)
{
    static cxConstChars fragment =
    GLSL(
        varying mediump vec4 vFragmentColor;
        varying highp vec2 vTexCoord;
        uniform sampler2D uTexture0;
        void main() {
            vec4 texColor = texture2D(uTexture0, vTexCoord);
            if(kxAtlasTexture)texColor.a=texture2D(uTexture0,vec2(vTexCoord.x,vTexCoord.y+0.5)).r;
            gl_FragColor = vec4(vFragmentColor.rgb,vFragmentColor.a * texColor.a);
        }
    );
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