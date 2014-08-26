//
//  cxMultiple.c
//  cxEngine
//
//  Created by xuhua on 10/29/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxMultiple.h"

static void cxMultipleRunNext(cxAny pav);

static void cxActionItemStop(cxAny sender)
{
    cxMultiple this = cxActionParent(sender);
    CX_ASSERT(this != NULL, "event arg not set");
    this->index ++;
    if(this->type == cxMultipleTypeSequence){
        cxMultipleRunNext(this);
    }
}

static void cxMultipleRunNext(cxAny pav)
{
    cxMultiple this = pav;
    if(this->index >= 0 && this->index < cxArrayLength(this->items)){
        cxAction action = cxArrayAtIndex(this->items, this->index);
        CX_EVENT_APPEND(action->onStop, cxActionItemStop);
        cxViewAppendAction(this->cxAction.view, action);
    }
}

static void cxMultipleRunAll(cxAny pav)
{
    cxMultiple this = pav;
    CX_ARRAY_FOREACH(this->items, ele){
        cxAction action = cxArrayObject(ele);
        CX_EVENT_APPEND(action->onStop, cxActionItemStop);
        cxViewAppendAction(this->cxAction.view, action);
    }
}

static void cxMultipleInit(cxAny pav)
{
    cxMultiple this = pav;
    CX_ASSERT(this->cxAction.view != NULL, "view not set");
    if(this->type == cxMultipleTypeSequence){
        this->index = 0;
        cxMultipleRunNext(this);
    }else if(this->type == cxMultipleTypeConcurrent){
        this->index = 0;
        cxMultipleRunAll(this);
    }
}

static cxBool cxMultipleExit(cxAny pav)
{
    cxMultiple this = pav;
    if(this->index >= cxArrayLength(this->items)){
        cxArrayClean(this->items);
        return true;
    }
    return false;
}

static void cxMultipleStep(cxAny pav,cxFloat dt,cxFloat time)
{
    //    cxActionRoot this = pav;
    //    CX_LOGGER("%f %f",this->super.duration,time);
}

CX_SETTER_DEF(cxMultiple, settype)
{
    cxConstChars setType = cxJsonToConstChars(value);
    if(cxConstCharsEqu(setType, "concurrent")){
        this->type = cxMultipleTypeConcurrent;
    }else if(cxConstCharsEqu(setType, "sequence")){
        this->type = cxMultipleTypeSequence;
    }else{
        this->type = cxMultipleTypeConcurrent;
    }
}
CX_SETTER_DEF(cxMultiple, actions)
{
    cxJson actions = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(actions, item)
    {
        cxAny action = cxObjectLoadWithJson(item);
        CX_ASSERT(CX_INSTANCE_OF(action, cxAction), "actions must is cxAction type");
        cxMultipleAppend(this, action);
    }
    CX_JSON_ARRAY_EACH_END(actions, item)
}

CX_OBJECT_TYPE(cxMultiple, cxAction)
{
    CX_PROPERTY_SETTER(cxMultiple, settype);
    CX_PROPERTY_SETTER(cxMultiple, actions);
}
CX_OBJECT_INIT(cxMultiple, cxAction)
{
    CX_METHOD_SET(this->cxAction.Init, cxMultipleInit);
    CX_METHOD_SET(this->cxAction.Step, cxMultipleStep);
    CX_METHOD_SET(this->cxAction.Exit, cxMultipleExit);
    this->items = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxMultiple, cxAction)
{
    CX_RELEASE(this->items);
}
CX_OBJECT_TERM(cxMultiple, cxAction)

void cxMultipleSetType(cxAny pav,cxMultipleType type)
{
    cxMultiple this = pav;
    this->type = type;
}

void cxMultipleAppend(cxAny pav,cxAny action)
{
    cxMultiple this = pav;
    cxArrayAppend(this->items, action);
    cxActionSetParent(action, pav);
}






