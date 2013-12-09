//
//  cxActionSet.c
//  cxEngine
//
//  Created by xuhua on 10/29/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxActionXML.h>
#include <core/cxEventArg.h>
#include "cxActionSet.h"

static void cxActionSetXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionXMLReadAttr(xmlAction, mAction, reader);
    cxActionSet this = mAction;
    //get type
    cxChar *stype = cxXMLAttr("cxActionSet.type");
    if(cxConstCharsEqu(stype, "multiple")){
        cxActionSetSetType(this, cxActionSetTypeMultiple);
    }else if(cxConstCharsEqu(stype, "sequence")){
        cxActionSetSetType(this, cxActionSetTypeSequence);
    }else{
        cxActionSetSetType(this, cxActionSetTypeNone);
    }
    xmlFree(stype);
    //get sub action
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(temp == NULL){
            continue;
        }
        cxAny action = cxActionXMLMakeElement(temp, reader);
        if(action == NULL){
            continue;
        }
        cxObjectXMLReadAttrRun(action, xmlAction, reader);
        cxActionSetAppend(this, action);
    }
}

static void cxActionSetRunNext(cxAny pav);

static void cxActionItemStop(cxEvent *event)
{
    cxActionSet this = cxEventArgToWeakRef(event->args);
    CX_ASSERT(this != NULL, "event arg not set");
    this->index ++;
    if(this->type == cxActionSetTypeSequence){
        cxActionSetRunNext(this);
    }
}

static void cxActionSetRunNext(cxAny pav)
{
    cxActionSet this = pav;
    if(this->index >= 0 && this->index < cxArrayLength(this->items)){
        cxAction action = cxArrayAtIndex(this->items, this->index);
        CX_EVENT_APPEND(action->onStop, cxActionItemStop, cxEventArgWeakRef(this));
        cxViewAppendAction(this->super.view, action);
    }
}

static void cxActionSetRunAll(cxAny pav)
{
    cxActionSet this = pav;
    CX_ARRAY_FOREACH(this->items, ele){
        cxAction action = cxArrayObject(ele);
        CX_EVENT_APPEND(action->onStop, cxActionItemStop, cxEventArgWeakRef(this));
        cxViewAppendAction(this->super.view, action);
    }
}

static void cxActionSetInit(cxAny pav)
{
    cxActionSet this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    if(this->type == cxActionSetTypeSequence){
        this->index = 0;
        cxActionSetRunNext(this);
        return;
    }
    if(this->type == cxActionSetTypeMultiple){
        this->index = 0;
        cxActionSetRunAll(this);
        return;
    }
}

static cxBool cxActionSetExit(cxAny pav)
{
    cxActionSet this = pav;
    if(this->index >= cxArrayLength(this->items)){
        cxArrayClean(this->items);
        return true;
    }
    return false;
}

static void cxActionSetStep(cxAny pav,cxFloat dt,cxFloat time)
{
    //    cxActionXML this = pav;
    //    CX_LOGGER("%f %f",this->super.duration,time);
}

CX_OBJECT_INIT(cxActionSet, cxAction)
{
    cxObjectSetXMLReadFunc(this, cxActionSetXMLReadAttr);
    CX_METHOD_SET(this->super.Init, cxActionSetInit);
    CX_METHOD_SET(this->super.Step, cxActionSetStep);
    CX_METHOD_SET(this->super.Exit, cxActionSetExit);
    this->items = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxActionSet, cxAction)
{
    CX_RELEASE(this->items);
}
CX_OBJECT_TERM(cxActionSet, cxAction)

void cxActionSetSetType(cxAny pav,cxActionSetType type)
{
    cxActionSet this = pav;
    this->type = type;
}

void cxActionSetAppend(cxAny pav,cxAny action)
{
    cxActionSet this = pav;
    cxArrayAppend(this->items, action);
}






