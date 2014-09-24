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
    CX_ASSERT_TYPE(sender, cxAction);
    cxAny parent = cxActionParent(sender);
    CX_ASSERT_THIS(parent, cxMultiple);
    this->index ++;
    if(this->type == cxMultipleTypeSequence){
        cxMultipleRunNext(this);
    }
}

static void cxMultipleRunNext(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxMultiple);
    if(this->index >= 0 && this->index < cxArrayLength(this->items)){
        cxAction action = cxArrayAtIndex(this->items, this->index);
        CX_EVENT_APPEND(action->onExit, cxActionItemStop);
        cxViewAppendAction(this->cxAction.view, action);
    }
}

static void cxMultipleRunAll(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxMultiple);
    CX_ARRAY_FOREACH(this->items, ele){
        cxAction action = cxArrayObject(ele);
        CX_EVENT_APPEND(action->onExit, cxActionItemStop);
        cxViewAppendAction(this->cxAction.view, action);
    }
}

static void cxMultipleInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxMultiple);
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
    CX_ASSERT_THIS(pav, cxMultiple);
    if(this->index >= cxArrayLength(this->items)){
        cxArrayClean(this->items);
        return true;
    }
    return false;
}

static void cxMultipleStep(cxAny pav,cxFloat dt,cxFloat time)
{
    //    cxActionRoot this = pav;
    //    CX_LOGGER("%f %f",this->time,time);
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
        cxAny object = cxObjectCreateWithJson(item);
        CX_ASSERT_TYPE(object, cxAction);
        cxMultipleAppend(this, object);
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
    CX_ASSERT_THIS(pav, cxMultiple);
    this->type = type;
}

void cxMultipleAppend(cxAny pav,cxAny action)
{
    CX_ASSERT_THIS(pav, cxMultiple);
    cxArrayAppend(this->items, action);
    cxActionSetParent(action, pav);
}






