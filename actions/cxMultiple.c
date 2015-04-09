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
    if(this->Type == cxMultipleTypeSequence){
        cxMultipleRunNext(this);
    }
}

static void cxMultipleRunNext(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxMultiple);
    if(this->index >= 0 && this->index < cxArrayLength(this->items)){
        cxAction action = cxArrayAtIndex(this->items, this->index);
        CX_ADD(cxAction, action, onExit, cxActionItemStop);
        cxViewAppendAction(cxActionGetView(this), action);
    }
}

static void cxMultipleRunAll(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxMultiple);
    CX_ARRAY_FOREACH(this->items, ele){
        cxAction action = cxArrayObject(ele);
        CX_ADD(cxAction, action, onExit, cxActionItemStop);
        cxViewAppendAction(cxActionGetView(this), action);
    }
}

CX_METHOD_DEF(cxMultiple,Init,void)
{
    CX_ASSERT_TYPE(cxActionGetView(this), cxView);
    if(this->Type == cxMultipleTypeSequence){
        this->index = 0;
        cxMultipleRunNext(this);
    }else if(this->Type == cxMultipleTypeConcurrent){
        this->index = 0;
        cxMultipleRunAll(this);
    }
    CX_SUPER(cxAction, this, Init, CX_M(void));
}
CX_METHOD_DEF(cxMultiple,IsExit,cxBool)
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
        this->Type = cxMultipleTypeConcurrent;
    }else if(cxConstCharsEqu(setType, "sequence")){
        this->Type = cxMultipleTypeSequence;
    }else{
        this->Type = cxMultipleTypeConcurrent;
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
    CX_METHOD(cxMultiple, IsExit);
}
CX_INIT(cxMultiple, cxAction)
{
    this->items = CX_ALLOC(cxArray);
    this->Type = cxMultipleTypeConcurrent;
}
CX_FREE(cxMultiple, cxAction)
{
    CX_RELEASE(this->items);
}
CX_TERM(cxMultiple, cxAction)

cxMultiple cxMultipleCreate(cxMultipleType type,...)
{
    cxMultiple this = CX_CREATE(cxMultiple);
    cxMultipleSetType(this, type);
    va_list ap;
    va_start(ap, type);
    cxAny pav = NULL;
    while((pav = va_arg(ap, cxAny)) != NULL) {
        cxMultipleAppend(this, pav);
    }
    va_end(ap);
    return this;
}

void cxMultipleAppend(cxAny pav,cxAny action)
{
    CX_ASSERT_THIS(pav, cxMultiple);
    CX_ASSERT_TYPE(action, cxAction);
    cxArrayAppend(this->items, action);
    cxActionSetParent(action, pav);
}






