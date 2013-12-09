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
    this->prevPos = this->begPos = this->super.view->position;
    kmVec2Subtract(&this->posDelta, &this->endPos, &this->begPos);
}

void cxMoveSetPos(cxAny pav,cxVec2f pos)
{
    cxMove this = pav;
    this->endPos = pos;
}

static void cxMoveStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxMove this = pav;
    cxVec2f npos;
    cxVec2f diff;
    kmVec2Subtract(&diff, &this->super.view->position, &this->prevPos);
    kmVec2Add(&this->begPos, &this->begPos, &diff);
    kmVec2Scale(&npos, &this->posDelta, time);
    kmVec2Add(&npos, &this->begPos, &npos);
    this->prevPos = npos;
    cxViewSetPos(this->super.view, npos);
}

static void cxMoveXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionXMLReadAttr(xmlAction, mAction, reader);
    cxMove this = mAction;
    this->endPos.x = cxXMLReadFloatAttr(reader, "cxMove.x", this->endPos.x);
    this->endPos.y = cxXMLReadFloatAttr(reader, "cxMove.y", this->endPos.y);
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

cxMove cxMoveCreate(cxFloat dutation, cxVec2f endpos)
{
    cxMove this = CX_CREATE(cxMove);
    this->super.duration = dutation;
    this->endPos = endpos;
    return this;
}
