//
//  cxSpline.c
//  cxEngine
//
//  Created by xuhua on 12/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxActionXML.h>
#include "cxSpline.h"

static void cxSplineInit(cxAny pav)
{
    cxSpline this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
}

static void cxSplineStep(cxAny pav,cxFloat dt,cxFloat time)
{
    
}

static void cxSplineXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionXML xml = xmlAction;
    cxActionXMLReadAttr(xmlAction, mAction, reader);
}

static void cxSplineReset(cxAny pav)
{
    cxSpline this = pav;
}

CX_OBJECT_INIT(cxSpline, cxAction)
{
    cxObjectSetXMLReadFunc(this, cxSplineXMLReadAttr);
    CX_METHOD_SET(this->super.Init, cxSplineInit);
    CX_METHOD_SET(this->super.Step, cxSplineStep);
    CX_METHOD_SET(this->super.Reset, cxSplineReset);
}
CX_OBJECT_FREE(cxSpline, cxAction)
{

}
CX_OBJECT_TERM(cxSpline, cxAction)