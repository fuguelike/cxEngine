//
//  cxAction.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include "cxActionRoot.h"
#include "cxAction.h"

static cxInt cxActionLuaAppendEvent(lua_State *L)
{
    CX_LUA_EVENT_BEG(cxAction);
    CX_LUA_EVENT_APPEND(onSplit);
    CX_LUA_EVENT_APPEND(onStart);
    CX_LUA_EVENT_APPEND(onStep);
    CX_LUA_EVENT_APPEND(onStop);
    CX_LUA_EVENT_END(cxAction);
}

CX_LUA_METHOD_BEG(cxAction)
    {"AppendEvent",cxActionLuaAppendEvent},
CX_LUA_METHOD_END(cxAction)

void cxActionTypeInit()
{
    CX_LUA_LOAD_TYPE(cxAction);
}

cxBool cxActionForever(cxAny pav)
{
    return false;
}

void cxActionReadAttr(cxReaderAttrInfo *info)
{
    cxAction this = info->object;
    //
    cxActionSetStepHide(this, cxXMLReadBoolAttr(info, "cxAction.stepHide", this->stepHide));
    //delay
    cxActionSetDelay(this, cxXMLReadFloatAttr(info, "cxAction.delay", this->delay));
    //time
    cxFloat time = cxXMLReadFloatAttr(info, "cxAction.time", this->duration);
    cxActionSetDuration(this, time);
    //curve
    cxConstChars scurve = cxXMLAttr(info->reader, "cxAction.curve");
    cxCurveItem curve = cxCurveGet(scurve);
    if(curve != NULL){
        CX_METHOD_OVERRIDE(this->Curve, curve->func);
    }
    //
    cxActionSetSplit(this, cxXMLReadIntAttr(info, "cxAction.split", this->split));
    //
    cxActionSetSpeed(this, cxXMLReadFloatAttr(info, "cxAction.speed", this->speed));
    //actionId
    cxActionSetId(this, cxXMLReadIntAttr(info, "cxAction.id", this->actionId));
    //forever
    if(cxXMLReadBoolAttr(info, "cxAction.forever", false)){
        CX_METHOD_OVERRIDE(this->Exit, cxActionForever);
    }
    //assist
    this->assist = cxXMLReadAssist4fAttr(info, "cxAction.assist", this->assist);
    //event
    cxXMLAppendEvent(info, this, cxAction, onStart);
    cxXMLAppendEvent(info, this, cxAction, onStop);
    cxXMLAppendEvent(info, this, cxAction, onSplit);
    cxXMLAppendEvent(info, this, cxAction, onStep);
}

void cxActionSetStepHide(cxAny pav,cxBool stepHide)
{
    cxAction this = pav;
    this->stepHide = stepHide;
}

void cxActionSetSplit(cxAny pav,cxInt split)
{
    cxAction this = pav;
    CX_RETURN(this->split == split);
    this->split = split;
    this->splitDelta = 1.0f / (cxFloat)(split - 1);
}

cxAny cxActionView(cxAny pav)
{
    cxAction this = pav;
    return this->view;
}

CX_OBJECT_INIT(cxAction, cxObject)
{
    this->super.cxBase = cxBaseTypeAction;
    cxObjectSetReadAttrFunc(this, cxActionReadAttr);
    this->isExit = false;
    this->speed = 1.0f;
    this->index = -1;
    this->split = -1;
    this->stepHide = false;
}
CX_OBJECT_FREE(cxAction, cxObject)
{
    CX_EVENT_RELEASE(this->onSplit);
    CX_EVENT_RELEASE(this->onStart);
    CX_EVENT_RELEASE(this->onStop);
    CX_EVENT_RELEASE(this->onStep);
    
    CX_METHOD_RELEASE(this->Curve);
    CX_METHOD_RELEASE(this->Init);
    CX_METHOD_RELEASE(this->Active);
    CX_METHOD_RELEASE(this->Over);
    CX_METHOD_RELEASE(this->Reset);
    CX_METHOD_RELEASE(this->Step);
    CX_METHOD_RELEASE(this->Exit);
}
CX_OBJECT_TERM(cxAction, cxObject)

void cxActionSetDuration(cxAny pav,cxFloat time)
{
    cxAction this = pav;
    this->duration = time;
}

void cxActionSetSpeed(cxAny pav,cxFloat speed)
{
    cxAction this = pav;
    this->speed = speed;
}

void cxActionSetCurve(cxAny pav,cxActionCurveFunc curve)
{
    cxAction this = pav;
    CX_METHOD_OVERRIDE(this->Curve, curve);
}

cxBool cxActionUpdate(cxAny pav,cxFloat dt)
{
    cxAction this = pav;
    cxBool isExit = false;
    dt = dt * this->speed;
    if(this->isPause || this->isExit){
        goto finished;
    }
    //action delay
    this->delayElapsed += dt;
    if(this->delay > 0 && this->delayElapsed < this->delay){
        goto finished;
    }
    //init event
    if(!this->isFirst){
        this->isFirst = true;
        CX_ASSERT(this->view != NULL, "action viewptr null");
        this->prevTime = 0;
        this->delayElapsed = 0;
        this->durationElapsed = 0;
        CX_METHOD_FIRE(NULL, this->Init, this);
        CX_EVENT_FIRE(this, onStart);
    }
    //for active
    if(!this->isActive){
        this->isActive = true;
        CX_METHOD_FIRE(NULL, this->Active, this);
    }
    this->durationElapsed += dt;
    cxFloat value = this->durationElapsed/CX_MAX(this->duration, FLT_EPSILON);
    cxFloat time = kmClamp(value, 0.0f, 1.0f);
    //split index event fire
    cxInt index = -1;
    if(time >= 1.0f){
        index = this->split - 1;
    }else if(this->splitDelta > 0){
        index = time / this->splitDelta;
    }
    if(this->index != index && index >= 0){
        this->index = index;
        CX_EVENT_FIRE(this,onSplit);
    }
    //wait exit
    if(this->duration < 0){
        CX_METHOD_FIRE(NULL, this->Step,this,dt,time);
        CX_EVENT_FIRE(this, onStep);
    }else if(this->duration == 0){
        isExit = CX_METHOD_FIRE(true, this->Exit, this);
    }else if(this->durationElapsed < this->duration){
        time = CX_METHOD_FIRE(time, this->Curve, this, time);
        this->delta = time - this->prevTime;
        this->prevTime = time;
        CX_METHOD_FIRE(NULL, this->Step,this,dt,time);
        CX_EVENT_FIRE(this, onStep);
    }else{
        time = CX_METHOD_FIRE(1.0f, this->Curve,this,1.0f);
        this->delta = time - this->prevTime;
        this->prevTime = 0;
        this->durationElapsed = 0.0f;
        this->delayElapsed = 0.0f;
        CX_METHOD_FIRE(NULL, this->Step,this,dt,1.0f);
        CX_EVENT_FIRE(this, onStep);
        //check exit
        isExit = CX_METHOD_FIRE(true, this->Exit,this);
        this->isActive = false;
    }
    //check action exit
finished:
    //force exit or auto exit
    if(this->isExit || isExit){
        CX_EVENT_FIRE(this, onStop);
        CX_METHOD_FIRE(NULL, this->Over,this);
    }
    return (this->isExit || isExit);
}

void cxActionPause(cxAny pav)
{
    cxAction this = pav;
    this->isPause = true;
}

void cxActionReset(cxAny pav)
{
    cxAction this = pav;
    this->isFirst = false;
    this->isExit = false;
    this->index = -1;
    this->split = -1;
    this->durationElapsed = 0;
    this->delayElapsed = 0;
    CX_EVENT_RELEASE(this->onSplit);
    CX_EVENT_RELEASE(this->onStart);
    CX_EVENT_RELEASE(this->onStop);
    CX_EVENT_RELEASE(this->onStep);
    CX_METHOD_FIRE(NULL, this->Reset, this);
}

void cxActionResume(cxAny pav)
{
    cxAction this = pav;
    this->isPause = false;
}

void cxActionStop(cxAny pav)
{
    cxEngine engine = cxEngineInstance();
    cxAction this = pav;
    this->isExit = true;
    cxActionUpdate(this, engine->frameDelta);
}

cxUInt cxActionGetId(cxAny pav)
{
    cxAction this = pav;
    return this->actionId != 0 ? this->actionId : (cxUInt)pav;
}

void cxActionSetDelay(cxAny pav,cxFloat delay)
{
    cxAction this = pav;
    this->delay = delay;
}

void cxActionSetId(cxAny pav,cxUInt actionId)
{
    cxAction this = pav;
    this->actionId = actionId;
}


