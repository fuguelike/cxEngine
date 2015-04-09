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

CX_INLINE void cxActionExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_CALL(this, Exit, CX_M(void));
    if(this->AutoRemove && this->View != NULL){
        cxViewRemove(this->View);
    }
}

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
    cxUInt id = (cxUInt)cxJsonToInt(value, this->Id);
    cxActionSetId(this, id);
}
CX_SETTER_DEF(cxAction, group)
{
    CX_ASSERT(cxJsonIsStr(value), "set action mgr group name");
    cxConstChars name = cxJsonToConstChars(value);
    CX_ASSERT(name != NULL, "name null");
    cxActionSetGroup(this, name);
}
CX_SETTER_DEF(cxAction, tag)
{
    cxUInt tag = cxJsonToInt(value, this->Tag);
    cxActionSetTag(this, tag);
}
CX_SETTER_DEF(cxAction, autoremove)
{
    cxActionSetAutoRemove(this, cxJsonToBool(value, this->AutoRemove));
}
CX_INLINE void cxActionResetElapsed(cxAction this)
{
    this->TimeElapsed = 0;
    this->DelayElapsed = 0;
}
CX_METHOD_DEF(cxAction, Init,void)
{
    this->DelayElapsed = 0;
    this->TimeElapsed = this->InitTime;
    CX_EVENT_FIRE(this, onInit);
}
CX_METHOD_DEF(cxAction, Reset,void)
{
    this->isFirst = false;
    this->isExit = false;
    cxActionResetElapsed(this);
}
CX_METHOD_DEF(cxAction, IsExit,cxBool)
{
    return true;
}
CX_METHOD_DEF(cxAction,Step,void,cxFloat dt, cxFloat time)
{
    
}
CX_METHOD_DEF(cxAction, Update, void)
{
    CX_EVENT_FIRE(this, onUpdate);
}
CX_METHOD_DEF(cxAction, Exit, void)
{
    CX_EVENT_FIRE(this, onExit);
}
CX_METHOD_DEF(cxAction, OnRun, cxBool ,cxFloat dt)
{
    cxBool isExit = false;
    cxBool isPause = false;
    //group process
    if(this->Group != NULL){
        dt *= (this->Scale * cxGroupGetScale(this->Group));
        isPause = cxGroupGetIsPause(this->Group);
    }else{
        dt *= this->Scale;
    }
    if(cxFloatEqu(dt, 0.0f)){
        goto finished;
    }
    //if pause or exit
    if(this->isPause || this->isExit || isPause){
        goto finished;
    }
    //action delay at init before
    this->DelayElapsed += dt;
    if(this->Delay > 0 && this->DelayElapsed < this->Delay){
        goto finished;
    }
    //init event
    if(!this->isFirst){
        this->isFirst = true;
        CX_CALL(this, Init, CX_M(void));
    }
    if(this->isExit){
        goto finished;
    }
    this->TimeElapsed += dt;
    cxFloat time = kmClamp(this->TimeElapsed/this->Time, 0.0f, 1.0f);
    time = this->Curve ? this->Curve(this, time) : time;
    if(this->Time < 0){
        CX_CALL(this, Step, CX_M(void,cxFloat,cxFloat),dt,time);
    }else if(cxFloatEqu(this->Time, 0.0f)){
        isExit = CX_CALL(this, IsExit, CX_M(cxBool));
    }else if(this->TimeElapsed < this->Time){
        CX_CALL(this, Step, CX_M(void,cxFloat,cxFloat),dt,time);
    }else{
        CX_CALL(this, Step, CX_M(void,cxFloat,cxFloat),dt,1.0f);
        isExit = CX_CALL(this, IsExit, CX_M(cxBool));
        cxActionResetElapsed(this);
    }
finished:
    //update
    CX_CALL(this, Update, CX_M(void));
    //force exit or auto exit
    if(this->isExit || isExit){
        cxActionExit(this);
    }
    return (this->isExit || isExit);
}
CX_TYPE(cxAction, cxObject)
{
    CX_METHOD(cxAction, Init);
    CX_METHOD(cxAction, Reset);
    CX_METHOD(cxAction, IsExit);
    CX_METHOD(cxAction, Step);
    CX_METHOD(cxAction, Update);
    CX_METHOD(cxAction, Exit);
    CX_METHOD(cxAction, OnRun);
    
    CX_SETTER(cxAction, autoremove);
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
}
CX_FREE(cxAction, cxObject)
{
    CX_RELEASE(this->Group);
    CX_EVENT_RELEASE(this->onInit);
    CX_EVENT_RELEASE(this->onExit);
    CX_EVENT_RELEASE(this->onUpdate);
}
CX_TERM(cxAction, cxObject)

void cxActionHideView(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    cxViewSetIsVisible(this->View, false);
}

void cxActionShowView(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    cxViewSetIsVisible(this->View, true);
}

cxBool cxActionForever(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_UNUSED_PARAM(this);
    return false;
}

void cxActionSetGroup(cxAny pav,cxConstChars name)
{
    CX_ASSERT_THIS(pav, cxAction);
    cxGroup group = cxGroupGet(name);
    CX_ASSERT(group != NULL, "action group name %s not exists",name);
    CX_RETAIN_SWAP(this->Group, group);
}

void cxActionReset(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_CALL(this, Reset, CX_M(void));
}

cxBool cxActionRun(cxAny pav,cxFloat dt)
{
    CX_ASSERT_THIS(pav, cxAction);
    return CX_CALL(this, OnRun, CX_M(cxBool,cxFloat),dt);
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



