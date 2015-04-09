//
//  cxClipping.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxOpenGL.h>
#include <engine/cxEngine.h>
#include "cxClipping.h"

#define CX_STENCIL_MASK 0xFF

static cxBool refs[CX_STENCIL_MASK + 1] = {0};

static cxUInt8 cxStencilRefAlloc()
{
    for(int i=1; i < CX_STENCIL_MASK; i++){
        if(!refs[i]){
            refs[i] = true;
            return i;
        }
    }
    CX_ERROR("cxClipping not stencil ref use,default use CX_STENCIL_MASK");
    return  CX_STENCIL_MASK;
}

static void cxStencilRefFree(cxInt index)
{
    refs[index] = false;
}

void cxClippingSetInverse(cxAny pview,cxBool inverse)
{
    CX_ASSERT_THIS(pview, cxClipping);
    this->inverse = inverse;
}

CX_METHOD_DEF(cxClipping,OnBefore,void)
{
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, this->useRef, CX_STENCIL_MASK);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    CX_CALL(this, OnClipping, CX_M(void));
    glStencilFunc(this->inverse ? GL_NOTEQUAL : GL_EQUAL, this->useRef, CX_STENCIL_MASK);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}
CX_METHOD_DEF(cxClipping,OnAfter,void)
{
    glDisable(GL_STENCIL_TEST);
}
CX_METHOD_DEF(cxClipping,OnClipping,void)
{
    
}
CX_TYPE(cxClipping, cxView)
{
    CX_METHOD(cxClipping, OnClipping);
    CX_METHOD(cxClipping, OnBefore);
    CX_METHOD(cxClipping, OnAfter);
}
CX_INIT(cxClipping, cxView)
{
    this->useRef = cxStencilRefAlloc();
    this->inverse = false;
}
CX_FREE(cxClipping, cxView)
{
    cxStencilRefFree(this->useRef);
}
CX_TERM(cxClipping, cxView)

cxTexture cxClippingTexture(cxAny pview)
{
    return cxOpenGLShader(cxShaderClippingKey);
}


