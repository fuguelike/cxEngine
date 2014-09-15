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

static void cxClippingDrawBefore(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxClipping);
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

static void cxClippingDrawAfter(cxAny pview)
{
    glDisable(GL_STENCIL_TEST);
}

CX_OBJECT_TYPE(cxClipping, cxView)
{
    
}
CX_OBJECT_INIT(cxClipping, cxView)
{
    this->useRef = cxStencilRefAlloc();
    CX_METHOD_SET(CX_TYPE(cxView, this)->Before, cxClippingDrawBefore);
    CX_METHOD_SET(CX_TYPE(cxView, this)->After, cxClippingDrawAfter);
}
CX_OBJECT_FREE(cxClipping, cxView)
{
    cxStencilRefFree(this->useRef);
    CX_EVENT_RELEASE(this->onClipping);
}
CX_OBJECT_TERM(cxClipping, cxView)




