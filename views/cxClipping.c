//
//  cxClipping.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxOpenGL.h>
#include <core/cxViewXML.h>
#include "cxClipping.h"

static GLuint cxUseLayer = 0;

static GLuint cxLayerIndex()
{
    GLuint mask = 0;
    for(int i=0; i < 32;i++){
        mask = 1 << i;
        if(cxUseLayer & mask){
            continue;
        }
        cxUseLayer |= mask;
        return i;
    }
    return 0;
}

static void cxUnsetLayerIndex(GLuint index)
{
    GLuint mask = 1 << index;
    cxUseLayer &= ~mask;
}

static void cxClippingDrawBefore(cxAny pview)
{
    cxClipping this = pview;
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1 << this->useLayer, 1 << this->useLayer);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    CX_EVENT_FIRE(this, onClipping);
    glStencilFunc(this->inverse?GL_NOTEQUAL:GL_EQUAL, 1 << this->useLayer, 1 << this->useLayer);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void cxClippingSetInverse(cxAny pview,cxBool inverse)
{
    cxClipping this = pview;
    this->inverse = inverse;
}

void cxClippingXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxViewXMLReadAttr(xmlView, mView, reader);
    cxViewXML xml = xmlView;
    cxClipping this = mView;
    cxClippingSetInverse(this, cxXMLReadBoolAttr(reader, "cxClipping.inverse", this->inverse));
    cxXMLAppendEvent(xml->events, this, cxClipping, onClipping);
}

static void cxClippingDrawAfter(cxAny pview)
{
    glDisable(GL_STENCIL_TEST);
}

CX_OBJECT_INIT(cxClipping, cxView)
{
    cxObjectSetXMLReadFunc(this, cxClippingXMLReadAttr);
    this->useLayer = cxLayerIndex();
    CX_METHOD_SET(this->super.DrawBefore, cxClippingDrawBefore);
    CX_METHOD_SET(this->super.DrawAfter, cxClippingDrawAfter);
}
CX_OBJECT_FREE(cxClipping, cxView)
{
    CX_EVENT_RELEASE(this->onClipping);
    cxUnsetLayerIndex(this->useLayer);
}
CX_OBJECT_TERM(cxClipping, cxView)


