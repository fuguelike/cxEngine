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
CX_SETTER_DEF(cxAction, actionid)
{
    cxUInt actionId = cxJsonToLong(value, this->ActionId);
    cxActionSetActionId(this, actionId);
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

CX_OBJECT_TYPE(cxAction, cxObject)
{
    CX_PROPERTY_SETTER(cxAction, time);
    CX_PROPERTY_SETTER(cxAction, delay);
    CX_PROPERTY_SETTER(cxAction, scale);
    CX_PROPERTY_SETTER(cxAction, curve);
    CX_PROPERTY_SETTER(cxAction, actionid);
    CX_PROPERTY_SETTER(cxAction, group);
    CX_PROPERTY_SETTER(cxAction, tag);
}
CX_OBJECT_INIT(cxAction, cxObject)
{
    this->Scale = 1.0f;
    this->isExit = false;
}
CX_OBJECT_FREE(cxAction, cxObject)
{
    CX_RELEASE(this->Group);
    CX_EVENT_RELEASE(this->onInit);
    CX_EVENT_RELEASE(this->onExit);
    CX_EVENT_RELEASE(this->onUpdate);
}
CX_OBJECT_TERM(cxAction, cxObject)

cxBool cxActionForever(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_UNUSED_PARAM(this);
    return false;
}

void cxActionSetCurve(cxAny pav,cxActionCurveFunc curve)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_SET(cxAction, this, Curve, curve);
}

void cxActionSetGroup(cxAny pav,cxConstChars name)
{
    CX_ASSERT_THIS(pav, cxAction);
    cxGroup mgr = cxGroupGet(name);
    CX_ASSERT(mgr != NULL, "action mgr name %s not exists",name);
    CX_RETAIN_SWAP(this->Group, mgr);
}

static void cxActionInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_ASSERT_TYPE(cxActionGetView(this), cxView);
    this->DelayElapsed = 0;
    this->TimeElapsed = this->InitTime;
    CX_METHOD_RUN(this->Init, this);
    CX_EVENT_FIRE(this, onInit);
}

static void cxActionExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_EVENT_FIRE(this, onExit);
    CX_METHOD_RUN(this->Over,this);
}

cxBool cxActionUpdate(cxAny pav,cxFloat dt)
{
    CX_ASSERT_THIS(pav, cxAction);
    cxBool isExit = false;
    //time scale
    if(this->Group != NULL){
        dt *= this->Scale * cxGroupGetScale(this->Group);
    }else{
        dt *= this->Scale;
    }
    //if pause
    if(this->PauseTime > 0){
        this->PauseTime -= dt;
        goto finished;
    }
    if(this->isPause){
        goto finished;
    }
    if(this->isExit){
        goto finished;
    }
    //init event
    if(!this->isFirst){
        this->isFirst = true;
        cxActionInit(pav);
    }
    if(this->isExit){
        goto finished;
    }
    //action delay
    this->DelayElapsed += dt;
    if(this->Delay > 0 && this->DelayElapsed < this->Delay){
        goto finished;
    }
    //for active
    if(!this->isActive){
        this->isActive = true;
        CX_METHOD_RUN(this->Active, this);
    }
    this->TimeElapsed += dt;
    cxFloat value = this->TimeElapsed/CX_MAX(this->Time, FLT_EPSILON);
    cxFloat time = kmClamp(value, 0.0f, 1.0f);
    //wait exit
    if(this->Time < 0){
        CX_METHOD_RUN(this->Step,this,dt,time);
    }else if(this->Time == 0){
        isExit = CX_METHOD_GET(true, this->Exit, this);
    }else if(this->TimeElapsed < this->Time){
        time = CX_METHOD_GET(time, this->Curve, this, time);
        CX_METHOD_RUN(this->Step,this,dt,time);
    }else{
        time = CX_METHOD_GET(1.0f, this->Curve,this,1.0f);
        CX_METHOD_RUN(this->Step,this,dt,1.0f);
        //check exit
        isExit = CX_METHOD_GET(true, this->Exit,this);
        this->TimeElapsed = 0.0f;
        this->DelayElapsed = 0.0f;
        this->isActive = false;
    }
finished:
    //update
    CX_EVENT_FIRE(this, onUpdate);
    //force exit or auto exit
    if(this->isExit || isExit){
        cxActionExit(this);
    }
    return (this->isExit || isExit);
}

void cxActionPause(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->isPause = true;
}

void cxActionReset(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->isFirst = false;
    this->isExit = false;
    this->TimeElapsed = 0;
    this->DelayElapsed = 0;
    CX_METHOD_RUN(this->Reset, this);
}

void cxActionResume(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->PauseTime = 0.0f;
    this->isPause = false;
}

void cxActionStop(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->isExit = true;
}



