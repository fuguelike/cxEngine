//
//  cxAction.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include "cxActionXML.h"
#include "cxAction.h"

cxBool cxActionForever(cxAny pav)
{
    return false;
}

void cxActionXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionXML xml = xmlAction;
    cxAction this = mAction;
    //delay
    cxActionSetDelay(mAction, cxXMLReadFloatAttr(reader, "cxAction.delay", this->delay));
    //speed
    cxFloat speed = cxXMLReadFloatAttr(reader, "cxAction.speed", 1.0f);
    cxFloat time = cxXMLReadFloatAttr(reader, "cxAction.time", this->duration);
    cxActionSetDuration(mAction, time/speed);
    //curve
    cxCurveItem curve = cxEngineGetCurve((cxConstChars)cxXMLAttr("cxAction.curve"));
    if(curve != NULL){
        CX_METHOD_SET(this->Curve, curve->func);
    }
    //
    cxActionSetScale(this, cxXMLReadFloatAttr(reader, "cxAction.scale", this->scale));
    //actionId
    cxActionSetId(this, cxXMLReadIntAttr(reader, "cxAction.id", this->actionId));
    //event
    cxXMLAppendEvent(xml->events, this, cxAction, onBefore);
    cxXMLAppendEvent(xml->events, this, cxAction, onAfter);
    //forever
    if(cxXMLReadBoolAttr(reader, "cxAction.forever", false)){
        CX_METHOD_SET(this->Exit, cxActionForever);
    }
}

CX_OBJECT_INIT(cxAction, cxObject)
{
    cxObjectSetXMLReadFunc(this, cxActionXMLReadAttr);
    this->isExit = false;
    this->scale = 1.0f;
}
CX_OBJECT_FREE(cxAction, cxObject)
{
    CX_EVENT_RELEASE(this->onAfter);
    CX_EVENT_RELEASE(this->onBefore);
}
CX_OBJECT_TERM(cxAction, cxObject)

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

void cxActionSetCurve(cxAny pav,cxActionCurveFunc curve)
{
    cxAction this = pav;
    CX_METHOD_SET(this->Curve, curve);
}

cxBool cxActionUpdate(cxAny pav,cxFloat dt)
{
    cxAction this = pav;
    cxBool isExit = false;
    if(this->isPause || this->isExit){
        goto finished;
    }
    dt = dt * this->scale;
    this->delayElapsed += dt;
    if(this->delay > 0 && this->delayElapsed < this->delay){
        goto finished;
    }
    if(!this->isFirst){
        this->isFirst = true;
        CX_ASSERT(this->view != NULL, "action viewptr null");
        this->prevTime = 0;
        this->delayElapsed = 0;
        this->durationElapsed = 0;
        CX_METHOD_RUN(this->Init,this);
        CX_EVENT_FIRE(this, onBefore);
    }
    this->durationElapsed += dt;
    cxFloat value = this->durationElapsed/CX_MAX(this->duration, FLT_EPSILON);
    cxFloat time = kmClamp(value, 0.0f, 1.0f);
    //wait exit for driver sub action
    if(this->duration == 0){
        isExit = CX_METHOD_GET(true, this->Exit,pav);
    }else if(this->durationElapsed < this->duration){
        time = CX_METHOD_GET(time, this->Curve, this, time);
        this->delta = time - this->prevTime;
        this->prevTime = time;
        CX_METHOD_RUN(this->Step,this,dt,time);
    }else{
        time = CX_METHOD_GET(1.0f, this->Curve, this, 1.0f);
        this->delta = time - this->prevTime;
        this->prevTime = 0;
        this->durationElapsed = 0.0f;
        this->delayElapsed = 0.0f;
        CX_METHOD_RUN(this->Step,this,dt,1.0f);
        isExit = CX_METHOD_GET(true, this->Exit,pav);
    }
finished:
    if(this->isExit || isExit){
        CX_EVENT_FIRE(this, onAfter);
        CX_METHOD_RUN(this->Over,this);
    }
    return (this->isExit || isExit);
}

void cxActionPause(cxAny pav)
{
    cxAction this = pav;
    this->isPause = true;
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


