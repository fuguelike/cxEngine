//
//  cxClipping.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxOpenGL.h>
#include <core/cxViewRoot.h>
#include <core/cxEngine.h>
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
    cxClipping this = pview;
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, this->useRef, CX_STENCIL_MASK);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    CX_EVENT_FIRE(this, onClipping);
    glStencilFunc(this->inverse ? GL_NOTEQUAL : GL_EQUAL, this->useRef, CX_STENCIL_MASK);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void cxClippingSetInverse(cxAny pview,cxBool inverse)
{
    cxClipping this = pview;
    this->inverse = inverse;
}

void cxClippingReadAttr(cxAny rootView,cxAny mView, xmlTextReaderPtr reader)
{
    cxViewReadAttr(rootView, mView, reader);
    cxViewRoot root = rootView;
    cxClipping this = mView;
    cxClippingSetInverse(this, cxXMLReadBoolAttr(reader,root->functions, "cxClipping.inverse", this->inverse));
    cxConstChars sitems = cxXMLAttr("cxClipping.boxes");
    if(sitems != NULL){
        CX_RETAIN_SWAP(this->boxes,cxEngineDataSet(sitems));
    }
    cxXMLAppendEvent(root->events, this, cxClipping, onClipping);
}

static void cxClippingDrawAfter(cxAny pview)
{
    glDisable(GL_STENCIL_TEST);
}

CX_OBJECT_INIT(cxClipping, cxView)
{
    this->useRef = cxStencilRefAlloc();
    cxObjectSetReadAttrFunc(this, cxClippingReadAttr);
    CX_METHOD_SET(this->super.DrawBefore, cxClippingDrawBefore);
    CX_METHOD_SET(this->super.DrawAfter, cxClippingDrawAfter);
}
CX_OBJECT_FREE(cxClipping, cxView)
{
    cxStencilRefFree(this->useRef);
    CX_RELEASE(this->boxes);
    CX_EVENT_RELEASE(this->onClipping);
}
CX_OBJECT_TERM(cxClipping, cxView)




