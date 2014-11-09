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
    cxAny parent = cxActionGetParent(sender);
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
        CX_ADD(cxAction, action, onExit, cxActionItemStop);
        cxViewAppendAction(cxActionGetView(action), action);
    }
}

static void cxMultipleRunAll(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxMultiple);
    CX_ARRAY_FOREACH(this->items, ele){
        cxAction action = cxArrayObject(ele);
        CX_ADD(cxAction, action, onExit, cxActionItemStop);
        cxViewAppendAction(cxActionGetView(action), action);
    }
}

CX_METHOD_DEF(cxMultiple,Init,void)
{
    CX_ASSERT_TYPE(cxActionGetView(this), cxView);
    if(this->type == cxMultipleTypeSequence){
        this->index = 0;
        cxMultipleRunNext(this);
    }else if(this->type == cxMultipleTypeConcurrent){
        this->index = 0;
        cxMultipleRunAll(this);
    }
}
CX_METHOD_DEF(cxMultiple,Exit,cxBool)
{
    if(this->index >= cxArrayLength(this->items)){
        cxArrayClear(this->items);
        return true;
    }
    return false;
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
        cxAny object = cxJsonMakeObject(item);
        CX_ASSERT_TYPE(object, cxAction);
        cxMultipleAppend(this, object);
    }
    CX_JSON_ARRAY_EACH_END(actions, item)
}

CX_TYPE(cxMultiple, cxAction)
{
    CX_SETTER(cxMultiple, settype);
    CX_SETTER(cxMultiple, actions);
    
    CX_METHOD(cxMultiple, Init);
    CX_METHOD(cxMultiple, Exit);
}
CX_INIT(cxMultiple, cxAction)
{
    this->items = CX_ALLOC(cxArray);
}
CX_FREE(cxMultiple, cxAction)
{
    CX_RELEASE(this->items);
}
CX_TERM(cxMultiple, cxAction)

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






