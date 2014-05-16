//
//  cxActionSet.c
//  cxEngine
//
//  Created by xuhua on 10/29/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxActionSet.h"

static void cxActionSetRunNext(cxAny pav);

static void cxActionItemStop(cxEvent *event)
{
    cxActionSet this = cxActionParent(event->sender);
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
        CX_EVENT_APPEND(action->onStop, cxActionItemStop);
        cxViewAppendAction(this->super.view, action);
    }
}

static void cxActionSetRunAll(cxAny pav)
{
    cxActionSet this = pav;
    CX_ARRAY_FOREACH(this->items, ele){
        cxAction action = cxArrayObject(ele);
        CX_EVENT_APPEND(action->onStop, cxActionItemStop);
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

void __cxActionSetInitType(cxAny type)
{
    
}

//void __cxActionSetInitObject(cxAny object,cxAny json,cxAny hash)
//{
//    cxActionSet this = object;
//    //set type
//    cxConstChars setType = cxJsonConstChars(json, "settype");
//    if(cxConstCharsEqu(setType, "multiple")){
//        this->type = cxActionSetTypeMultiple;
//    }else if(cxConstCharsEqu(setType, "sequence")){
//        this->type = cxActionSetTypeSequence;
//    }else{
//        this->type = cxActionSetTypeMultiple;
//    }
//    //load actions
//    cxJson actions = cxJsonArray(json, "actions");
//    CX_JSON_ARRAY_EACH_BEG(actions, item)
//    {
//        cxAny action = cxObjectLoadWithJson(item, hash);
//        CX_ASSERT(CX_INSTANCE_OF(action, cxAction), "actions must is cxAction type");
//        cxActionSetAppend(object, action);
//    }
//    CX_JSON_ARRAY_EACH_END(actions, item)
//    CX_OBJECT_INIT_SUPER(cxAction);
//}

CX_OBJECT_TYPE(cxActionSet, cxAction)
{
    
}
CX_OBJECT_INIT(cxActionSet, cxAction)
{
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
    cxActionSetParent(action, pav);
}






