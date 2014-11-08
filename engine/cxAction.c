//
//  cxAction.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include "cxAction.h"
#include "cxGroup.h"

CX_SETTER_DEF(cxAction, time)
{
    cxFloat time = cxJsonToDouble(value, this->Time);
    cxActionSetTime(this, time);
}
CX_SETTER_DEF(cxAction, delay)
{
    cxFloat delay = cxJsonToDouble(value, this->Delay);
    cxActionSetDelay(this, delay);
}
CX_SETTER_DEF(cxAction, scale)
{
    cxFloat scale = cxJsonToDouble(value, this->Scale);
    cxActionSetScale(this, scale);
}
CX_SETTER_DEF(cxAction, curve)
{
    cxConstChars curve = cxJsonToConstChars(value);
    cxCurveItem item = NULL;
    if(curve != NULL){
        item = cxCurveGet(curve);
    }
    if(item != NULL){
        cxActionSetCurve(this, item->func);
    }
}
CX_SETTER_DEF(cxAction, id)
{
    cxUInt id = (cxUInt)cxJsonToLong(value, this->Id);
    cxActionSetId(this, id);
}
CX_SETTER_DEF(cxAction, group)
{
    CX_ASSERT(cxJsonIsString(value), "set action mgr group name");
    cxConstChars name = cxJsonToConstChars(value);
    CX_ASSERT(name != NULL, "name null");
    cxActionSetGroup(this, name);
}
CX_SETTER_DEF(cxAction, tag)
{
    cxLong tag = cxJsonToLong(value, this->Tag);
    cxActionSetTag(this, tag);
}

static void CX_METHOD(cxAction, Init)
{
    this->DelayElapsed = 0;
    this->TimeElapsed = this->InitTime;
    CX_EVENT_FIRE(this, onInit);
}
CX_INLINE void cxActionResetElapsed(cxAction this)
{
    this->TimeElapsed = 0;
    this->DelayElapsed = 0;
}
static void CX_METHOD(cxAction, Reset)
{
    this->isFirst = false;
    this->isExit = false;
    cxActionResetElapsed(this);
}
static cxBool CX_METHOD(cxAction, Exit)
{
    return true;
}
static void CX_METHOD(cxAction, Step,cxFloat dt, cxFloat time)
{
    
}

CX_TYPE(cxAction, cxObject)
{
    CX_MSET(cxAction, Init);
    CX_MSET(cxAction, Reset);
    CX_MSET(cxAction, Exit);
    CX_MSET(cxAction, Step);
    
    CX_SETTER(cxAction, time);
    CX_SETTER(cxAction, delay);
    CX_SETTER(cxAction, scale);
    CX_SETTER(cxAction, curve);
    CX_SETTER(cxAction, id);
    CX_SETTER(cxAction, group);
    CX_SETTER(cxAction, tag);
}
CX_INIT(cxAction, cxObject)
{
    this->Scale = 1.0f;
    this->isExit = false;
}
CX_FREE(cxAction, cxObject)
{
    CX_RELEASE(this->Group);
    CX_EVENT_RELEASE(this->onInit);
    CX_EVENT_RELEASE(this->onExit);
    CX_EVENT_RELEASE(this->onUpdate);
}
CX_TERM(cxAction, cxObject)

cxBool cxActionForever(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_UNUSED_PARAM(this);
    return false;
}

void cxActionSetGroup(cxAny pav,cxConstChars name)
{
    CX_ASSERT_THIS(pav, cxAction);
    cxGroup mgr = cxGroupGet(name);
    CX_ASSERT(mgr != NULL, "action mgr name %s not exists",name);
    CX_RETAIN_SWAP(this->Group, mgr);
}

CX_INLINE void cxActionFireExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_EVENT_FIRE(this, onExit);
}

cxBool cxActionUpdate(cxAny pav,cxFloat dt)
{
    CX_ASSERT_THIS(pav, cxAction);
    cxBool isExit = false;
    //time scale
    if(this->Group != NULL){
        dt *= this->Scale * cxGroupGetScale(this->Group,pav);
    }else{
        dt *= this->Scale;
    }
    if(this->isPause || this->isExit){
        goto finished;
    }
    //action delay init before
    this->DelayElapsed += dt;
    if(this->Delay > 0 && this->DelayElapsed < this->Delay){
        goto finished;
    }
    //init event
    if(!this->isFirst){
        this->isFirst = true;
        CX_CALL(this, Init, CX_MT(void));
    }
    if(this->isExit){
        goto finished;
    }
    this->TimeElapsed += dt;
    cxFloat time = kmClamp(this->TimeElapsed/this->Time, 0.0f, 1.0f);
    time = this->Curve ? this->Curve(this, time) : time;
    if(this->Time < 0){
        CX_CALL(this, Step, CX_MT(void,cxFloat,cxFloat),dt,time);
    }else if(cxFloatEqu(this->Time, 0.0f)){
        isExit = CX_CALL(this, Exit, CX_MT(cxBool));
    }else if(this->TimeElapsed < this->Time){
        CX_CALL(this, Step, CX_MT(void,cxFloat,cxFloat),dt,time);
    }else{
        CX_CALL(this, Step, CX_MT(void,cxFloat,cxFloat),dt,1.0f);
        isExit = CX_CALL(this, Exit, CX_MT(cxBool));
        cxActionResetElapsed(this);
    }
finished:
    //update
    CX_EVENT_FIRE(this, onUpdate);
    //force exit or auto exit
    if(this->isExit || isExit){
        cxActionFireExit(this);
    }
    return (this->isExit || isExit);
}

void cxActionPause(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->isPause = true;
}

void cxActionResume(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->isPause = false;
}

void cxActionStop(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->isExit = true;
}



