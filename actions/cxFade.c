//
//  cxFade.c
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxActionXML.h>
#include "cxFade.h"

static void cxFadeInit(cxAny pav)
{
    cxFade this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    this->start = this->super.view->color.a;
    this->delta = this->alpha - this->start;
}

static void cxFadeStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxFade this = pav;
    cxFloat alpha = this->start + time * this->delta;
    cxViewSetAlpha(this->super.view, alpha);
}

static void cxFadeXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionXML xml = xmlAction;
    cxActionXMLReadAttr(xmlAction, mAction, reader);
    cxFade this = mAction;
    this->alpha = cxXMLReadFloatAttr(reader, xml->functions, "cxFade.alpha", this->alpha);
}

CX_OBJECT_INIT(cxFade, cxAction)
{
    cxObjectSetXMLReadFunc(this, cxFadeXMLReadAttr);
    CX_METHOD_SET(this->super.Init, cxFadeInit);
    CX_METHOD_SET(this->super.Step, cxFadeStep);
}
CX_OBJECT_FREE(cxFade, cxAction)
{

}
CX_OBJECT_TERM(cxFade, cxAction)

cxFade cxFadeCreate(cxFloat duration,cxFloat alpha)
{
    cxFade this = CX_CREATE(cxFade);
    this->super.duration = duration;
    this->alpha = alpha;
    return this;
}

