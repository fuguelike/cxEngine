//
//  cxAction.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include "cxAction.h"

CX_SETTER_DEF(cxAction, duration)
{
    this->duration = cxJsonToDouble(value, this->duration);
}
CX_SETTER_DEF(cxAction, delay)
{
    this->delay = cxJsonToDouble(value, this->delay);
}
CX_SETTER_DEF(cxAction, scale)
{
    this->scale = cxJsonToDouble(value, this->scale);
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
    this->actionId = cxJsonToLong(value, this->actionId);
}
CX_SETTER_DEF(cxAction, index)
{
    cxInt indexnum = cxJsonToInt(value, this->indexNum);
    cxActionSetIndex(this, indexnum);
}

CX_OBJECT_TYPE(cxAction, cxObject)
{
    CX_PROPERTY_SETTER(cxAction, duration);
    CX_PROPERTY_SETTER(cxAction, delay);
    CX_PROPERTY_SETTER(cxAction, scale);
    CX_PROPERTY_SETTER(cxAction, curve);
    CX_PROPERTY_SETTER(cxAction, actionid);
    CX_PROPERTY_SETTER(cxAction, index);
}
CX_OBJECT_INIT(cxAction, cxObject)
{
    this->scale = 1.0f;
    this->isExit = false;
    this->index = -1;
    this->indexNum = -1;
}
CX_OBJECT_FREE(cxAction, cxObject)
{
    CX_EVENT_RELEASE(this->onIndex);
    CX_EVENT_RELEASE(this->onStart);
    CX_EVENT_RELEASE(this->onStop);
    CX_EVENT_RELEASE(this->onStep);
}
CX_OBJECT_TERM(cxAction, cxObject)

cxBool cxActionForever(cxAny pav)
{
    return false;
}

void cxActionSetIndex(cxAny pav,cxInt indexNum)
{
    cxAction this = pav;
    CX_RETURN(this->indexNum == indexNum);
    this->indexNum = indexNum;
    this->indexDelta = 1.0f / (cxFloat)(indexNum - 1);
}

cxAny cxActionView(cxAny pav)
{
    cxAction this = pav;
    return this->view;
}

void cxActionSetView(cxAny pav,cxAny pview)
{
    cxAction this = pav;
    this->view = pview;
}

void cxActionSetDuration(cxAny pav,cxFloat time)
{
    cxAction this = pav;
    this->duration = time;
}

void cxActionSetScale(cxAny pav,cxFloat scale)
{
    cxAction this = pav;
    this->scale = scale;
}

void cxActionSetDurationInit(cxAny pav,cxFloat time)
{
    cxAction this = pav;
    this->durationInit = time;
}

void cxActionSetCurve(cxAny pav,cxActionCurveFunc curve)
{
    cxAction this = pav;
    CX_METHOD_SET(this->Curve, curve);
}

cxAny cxActionParent(cxAny pav)
{
    cxAction this = pav;
    return this->parent;
}

void cxActionSetParent(cxAny pav,cxAny parent)
{
    cxAction this = pav;
    this->parent = parent;
}

void cxActionSetPauseTime(cxAny pav,cxFloat time)
{
    cxAction this = pav;
    this->pauseTime = time;
}

cxBool cxActionUpdate(cxAny pav,cxFloat dt)
{
    cxAction this = pav;
    cxBool isExit = false;
    dt = dt * this->scale;
    if(this->pauseTime > 0){
        this->pauseTime -= dt;
        CX_EVENT_FIRE(this, onStep);
        goto finished;
    }
    if(this->isPause){
        CX_EVENT_FIRE(this, onStep);
        goto finished;
    }
    if(this->isExit){
        goto finished;
    }
    //init event
    if(!this->isFirst){
        this->isFirst = true;
        CX_ASSERT(this->view != NULL, "action viewptr null");
        this->prevTime = 0;
        this->delayElapsed = 0;
        this->durationElapsed = this->durationInit;
        CX_METHOD_RUN(this->Init, this);
        CX_EVENT_FIRE(this, onStart);
    }
    //action delay
    this->delayElapsed += dt;
    if(this->delay > 0 && this->delayElapsed < this->delay){
        CX_EVENT_FIRE(this, onStep);
        goto finished;
    }
    //for active
    if(!this->isActive){
        this->isActive = true;
        CX_METHOD_RUN(this->Active, this);
    }
    this->durationElapsed += dt;
    cxFloat value = this->durationElapsed/CX_MAX(this->duration, FLT_EPSILON);
    cxFloat time = kmClamp(value, 0.0f, 1.0f);
    //split index event fire
    cxInt index = -1;
    if(time >= 1.0f){
        index = this->indexNum - 1;
    }else if(this->indexDelta > 0){
        index = time / this->indexDelta;
    }
    if(this->index != index && index >= 0){
        this->index = index;
        CX_EVENT_FIRE(this,onIndex);
    }
    //wait exit
    if(this->duration < 0){
        CX_METHOD_RUN(this->Step,this,dt,time);
        CX_EVENT_FIRE(this, onStep);
    }else if(this->duration == 0){
        isExit = CX_METHOD_GET(true, this->Exit, this);
    }else if(this->durationElapsed < this->duration){
        time = CX_METHOD_GET(time, this->Curve, this, time);
        this->delta = time - this->prevTime;
        this->prevTime = time;
        CX_METHOD_RUN(this->Step,this,dt,time);
        CX_EVENT_FIRE(this, onStep);
    }else{
        time = CX_METHOD_GET(1.0f, this->Curve,this,1.0f);
        this->delta = time - this->prevTime;
        this->prevTime = 0;
        this->durationElapsed = 0.0f;
        this->delayElapsed = 0.0f;
        CX_METHOD_RUN(this->Step,this,dt,1.0f);
        CX_EVENT_FIRE(this, onStep);
        //check exit
        isExit = CX_METHOD_GET(true, this->Exit,this);
        this->isActive = false;
    }
    //check action exit
finished:
    //force exit or auto exit
    if(this->isExit || isExit){
        CX_EVENT_FIRE(this, onStop);
        CX_METHOD_RUN(this->Over,this);
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
    this->durationElapsed = 0;
    this->delayElapsed = 0;
    CX_METHOD_RUN(this->Reset, this);
}

void cxActionResume(cxAny pav)
{
    cxAction this = pav;
    this->pauseTime = 0.0f;
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


