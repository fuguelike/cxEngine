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

CX_METHOD_DEF(cxClipping,DrawBefore,void)
{
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, this->useRef, CX_STENCIL_MASK);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    CX_EVENT_FIRE(this, onClipping);
    
    glStencilFunc(this->inverse ? GL_NOTEQUAL : GL_EQUAL, this->useRef, CX_STENCIL_MASK);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void cxClippingSetInverse(cxAny pview,cxBool inverse)
{
    CX_ASSERT_THIS(pview, cxClipping);
    this->inverse = inverse;
}

CX_METHOD_DEF(cxClipping,DrawAfter,void)
{
    glDisable(GL_STENCIL_TEST);
}

CX_TYPE(cxClipping, cxView)
{
    CX_METHOD(cxClipping, DrawBefore);
    CX_METHOD(cxClipping, DrawAfter);
}
CX_INIT(cxClipping, cxView)
{
    this->useRef = cxStencilRefAlloc();
}
CX_FREE(cxClipping, cxView)
{
    cxStencilRefFree(this->useRef);
    CX_EVENT_RELEASE(this->onClipping);
}
CX_TERM(cxClipping, cxView)




