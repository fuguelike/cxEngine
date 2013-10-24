//
//  cxMove.c
//  cxEngine
//
//  Created by xuhua on 10/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxMove.h"
#include "cxActionXML.h"

static void cxMoveInit(cxAny pav)
{
    cxMove this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    this->oldPos = this->super.view->position;
    this->delta.x = this->newPos.x - this->oldPos.x;
    this->delta.y = this->newPos.y - this->oldPos.y;
}

static void cxMoveStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxMove this = pav;
    cxVec2f npos;
    npos.x = this->oldPos.x + this->delta.x * time;
    npos.y = this->oldPos.y + this->delta.y * time;
    cxViewSetPosition(this->super.view, npos);
}

static void cxMoveXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionXMLReadAttr(xmlAction, mAction, reader);
    cxMove this = mAction;
    this->newPos.x = cxXMLReadFloatAttr(reader, "cxMove.x", this->newPos.x);
    this->newPos.y = cxXMLReadFloatAttr(reader, "cxMove.y", this->newPos.y);
}

CX_OBJECT_INIT(cxMove, cxAction)
{
    cxObjectSetXMLReadFunc(this, cxMoveXMLReadAttr);
    CX_METHOD_SET(this->super.Init, cxMoveInit);
    CX_METHOD_SET(this->super.Step, cxMoveStep);
}
CX_OBJECT_FREE(cxMove, cxAction)
{
    
}
CX_OBJECT_TERM(cxMove, cxAction)

cxMove cxMoveCreate(cxFloat dutation, cxVec2f newpos)
{
    cxMove this = CX_CREATE(cxMove);
    this->super.duration = dutation;
    this->newPos = newpos;
    return this;
}
