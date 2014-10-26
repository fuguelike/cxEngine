//
//  cxShaderDefault.c
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxShaderDefault.h"

cxString cxShaderDefaultVertex(cxAny ps)
{
    static cxConstChars vertex =
    GLSL(
        attribute highp vec4 aPosition;
        attribute highp vec2 aTexcoord;
        attribute mediump vec4 aColor;
        varying mediump vec4 vFragmentColor;
        varying highp vec2 vTexCoord;
        void main() {
            gl_Position = cxMatrixModelViewProject * aPosition;
            vFragmentColor = aColor;
            vTexCoord = aTexcoord;
        }
    );
    return cxStringConstChars(vertex);
}

cxString cxShaderDefaultFragment(cxAny ps)
{
    static cxConstChars fragment =
    GLSL(
        varying mediump vec4 vFragmentColor;
        varying highp vec2 vTexCoord;
        uniform sampler2D uTexture0;
        void main() {
            gl_FragColor = vFragmentColor * texture2D(uTexture0, vTexCoord);
        }
    );
    return cxStringConstChars(fragment);
}

CX_TYPE(cxShaderDefault, cxShader)
{
    
}
CX_INIT(cxShaderDefault, cxShader)
{
    CX_SET(cxShader, this, Vertex, cxShaderDefaultVertex);
    CX_SET(cxShader, this, Fragment, cxShaderDefaultFragment);
}
CX_FREE(cxShaderDefault, cxShader)
{

}
CX_TERM(cxShaderDefault, cxShader)









