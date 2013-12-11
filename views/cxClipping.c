//
//  cxClipping.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxOpenGL.h>
#include <core/cxViewXML.h>
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

static void cxClippingBoxes(cxEvent *event)
{
    cxClipping this = event->sender;
    CX_TYPES_FOREACH(this->boxes, cxAtlasBoxPointType, tmp){
        cxAtlasBoxPointType box = *tmp;
        cxDrawClippingRect(box.pos, box.size);
    }
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
    cxClippingSetInverse(this, cxXMLReadBoolAttr(reader,xml->functions, "cxClipping.inverse", this->inverse));
    cxChar *sitems = cxXMLAttr("cxClipping.boxes");
    if(sitems != NULL){
        CX_RETAIN_SWAP(this->boxes,cxEngineDataSet(sitems));
    }
    xmlFree(sitems);
    cxXMLAppendEvent(xml->events, this, cxClipping, onClipping);
}

static void cxClippingDrawAfter(cxAny pview)
{
    glDisable(GL_STENCIL_TEST);
}

CX_OBJECT_INIT(cxClipping, cxView)
{
    CX_EVENT_APPEND(this->onClipping, cxClippingBoxes, NULL);
    this->useRef = cxStencilRefAlloc();
    cxObjectSetXMLReadFunc(this, cxClippingXMLReadAttr);
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




