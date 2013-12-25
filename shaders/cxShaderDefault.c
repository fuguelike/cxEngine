//
//  cxShaderDefault.c
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"

const luaL_Reg cxShaderDefaultInstanceMethods[] = {
    
    CX_LUA_SUPER(cxShader)
};

const luaL_Reg cxShaderDefaultTypeMethods[] = {
    CX_LUA_TYPE(cxShaderDefault)
};

void cxShaderDefaultTypeInit()
{
    CX_LUA_LOAD_TYPE(cxShaderDefault);
}

cxString cxShaderDefaultGetVertexSource(cxAny this)
{
    static cxConstChars vertex =
    GLSL(
        attribute highp vec4 aPosition;
        attribute highp vec2 aTexcoord;
        attribute mediump vec4 aColor;
        varying mediump vec4 vFragmentColor;
        varying highp vec2 vTexCoord;
        void main() {
            gl_Position = kxMatrixModelViewProject * aPosition;
            vFragmentColor = aColor;
            vTexCoord = aTexcoord;
        }
    );
    return cxStringConstChars(vertex);
}

cxString cxShaderDefaultGetFragmentSource(cxAny this)
{
    static cxConstChars fragment =
    GLSL
    (
        varying mediump vec4 vFragmentColor;
        varying highp vec2 vTexCoord;
        uniform sampler2D uTexture0;
        void main() {
            vec4 texColor = texture2D(uTexture0, vTexCoord);
            if(kxAtlasTexture){
                texColor.a = texture2D(uTexture0,vec2(vTexCoord.x,vTexCoord.y+0.5)).r;
            }
            gl_FragColor = vFragmentColor * texColor;
        }
    );
    return cxStringConstChars(fragment);
}

CX_OBJECT_INIT(cxShaderDefault, cxShader)
{
    CX_METHOD_OVERRIDE(this->super.GetVertexSource, cxShaderDefaultGetVertexSource);
    CX_METHOD_OVERRIDE(this->super.GetFragmentSource, cxShaderDefaultGetFragmentSource);
}
CX_OBJECT_FREE(cxShaderDefault, cxShader)
{

}
CX_OBJECT_TERM(cxShaderDefault, cxShader)









