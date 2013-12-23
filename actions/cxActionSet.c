//
//  cxActionSet.c
//  cxEngine
//
//  Created by xuhua on 10/29/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <core/cxXMLScript.h>
#include <core/cxActionRoot.h>
#include <core/cxEventArg.h>
#include "cxActionSet.h"

const luaL_Reg cxActionSetInstanceMethods[] = {
    
    CX_LUA_SUPER(cxAction)
};

const luaL_Reg cxActionSetTypeMethods[] = {
    
    CX_LUA_TYPE(cxActionSet)
};

void cxActionSetTypeInit()
{
    CX_LUA_LOAD_TYPE(cxActionSet);
}

static cxNumber cxActionType(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "arg error");
    cxConstChars mode = cxEventArgToString(arg);
    if(cxConstCharsEqu(mode, "multiple")){
        return cxNumberInt(cxActionSetTypeMultiple);
    }
    if(cxConstCharsEqu(mode, "sequence")){
        return cxNumberInt(cxActionSetTypeSequence);
    }
    return cxNumberInt(cxActionSetTypeNone);
}

void cxActionRootTypeInit()
{
    cxEngineRegisteTypeFunc(cxActionRootTypeName, "cxActionType", cxActionType);
}

static void cxActionSetReadAttr(cxAny rootAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionRoot root = rootAction;
    cxActionReadAttr(rootAction, mAction, reader);
    cxActionSet this = mAction;
    //get type use cxActionType('sequence') atr func
    cxActionSetSetType(this,cxXMLReadIntAttr(reader, root->functions, "cxActionSet.type", this->type));
    //get sub action
    while(xmlTextReaderRead(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        cxAny action = cxActionRootMakeElement(temp, reader);
        if(action == NULL){
            continue;
        }
        cxObjectReadAttrRun(action, rootAction, reader);
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
    //    cxActionRoot this = pav;
    //    CX_LOGGER("%f %f",this->super.duration,time);
}

CX_OBJECT_INIT(cxActionSet, cxAction)
{
    cxObjectSetReadAttrFunc(this, cxActionSetReadAttr);
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






