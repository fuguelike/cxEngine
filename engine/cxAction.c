//
//  cxAction.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include "cxAction.h"
#include "cxActionMgr.h"

CX_SETTER_DEF(cxAction, time)
{
    this->time = cxJsonToDouble(value, this->time);
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
CX_SETTER_DEF(cxAction, group)
{
    CX_ASSERT(cxJsonIsString(value), "set action mgr group name");
    cxConstChars name = cxJsonToConstChars(value);
    CX_ASSERT(name != NULL, "name null");
    cxActionSetGroup(this, name);
}
CX_SETTER_DEF(cxAction, tag)
{
    this->tag = cxJsonToLong(value, this->tag);
}

CX_OBJECT_TYPE(cxAction, cxObject)
{
    CX_PROPERTY_SETTER(cxAction, time);
    CX_PROPERTY_SETTER(cxAction, delay);
    CX_PROPERTY_SETTER(cxAction, scale);
    CX_PROPERTY_SETTER(cxAction, curve);
    CX_PROPERTY_SETTER(cxAction, actionid);
    CX_PROPERTY_SETTER(cxAction, index);
    CX_PROPERTY_SETTER(cxAction, group);
    CX_PROPERTY_SETTER(cxAction, tag);
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
    CX_RELEASE(this->actionMgr);
    CX_EVENT_RELEASE(this->onIndex);
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

void cxActionSetIndex(cxAny pav,cxInt indexNum)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_ASSERT(indexNum > 1, "indexNum small");
    CX_RETURN(this->indexNum == indexNum);
    this->indexNum = indexNum;
    this->indexDelta = 1.0f / (cxFloat)(indexNum - 1);
}

cxAny cxActionView(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    return this->view;
}

void cxActionSetView(cxAny pav,cxAny pview)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->view = pview;
}

void cxActionSetTime(cxAny pav,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->time = time;
}

void cxActionSetScale(cxAny pav,cxFloat scale)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->scale = scale;
}

void cxActionSetTimeInit(cxAny pav,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->timeInit = time;
}

void cxActionSetCurve(cxAny pav,cxActionCurveFunc curve)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_METHOD_SET(this->Curve, curve);
}

void cxActionSetGroup(cxAny pav,cxConstChars name)
{
    CX_ASSERT_THIS(pav, cxAction);
    cxActionMgr mgr = cxActionMgrGet(name);
    CX_ASSERT(mgr != NULL, "action mgr name %s not exists",name);
    cxActionSetMgr(this, mgr);
}

cxLong cxActionTag(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    return this->tag;
}

void cxActionSetTag(cxAny pav,cxLong tag)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->tag = tag;
}

void cxActionSetMgr(cxAny pav,cxActionMgr mgr)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_RETAIN_SWAP(this->actionMgr, mgr);
}

cxAny cxActionParent(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    return this->parent;
}

void cxActionSetParent(cxAny pav,cxAny parent)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->parent = parent;
}

void cxActionSetPauseTime(cxAny pav,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->pauseTime = time;
}

static void cxActionInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_ASSERT(this->view != NULL, "action viewptr null");
    this->delayElapsed = 0;
    this->timeElapsed = this->timeInit;
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
    if(this->actionMgr != NULL){
        dt *= this->scale * this->actionMgr->scale;
    }else{
        dt *= this->scale;
    }
    //if pause
    if(this->pauseTime > 0){
        this->pauseTime -= dt;
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
    this->delayElapsed += dt;
    if(this->delay > 0 && this->delayElapsed < this->delay){
        goto finished;
    }
    //for active
    if(!this->isActive){
        this->isActive = true;
        CX_METHOD_RUN(this->Active, this);
    }
    this->timeElapsed += dt;
    cxFloat value = this->timeElapsed/CX_MAX(this->time, FLT_EPSILON);
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
    if(this->time < 0){
        CX_METHOD_RUN(this->Step,this,dt,time);
    }else if(this->time == 0){
        isExit = CX_METHOD_GET(true, this->Exit, this);
    }else if(this->timeElapsed < this->time){
        time = CX_METHOD_GET(time, this->Curve, this, time);
        CX_METHOD_RUN(this->Step,this,dt,time);
    }else{
        time = CX_METHOD_GET(1.0f, this->Curve,this,1.0f);
        CX_METHOD_RUN(this->Step,this,dt,1.0f);
        //check exit
        isExit = CX_METHOD_GET(true, this->Exit,this);
        this->timeElapsed = 0.0f;
        this->delayElapsed = 0.0f;
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
    this->index = -1;
    this->timeElapsed = 0;
    this->delayElapsed = 0;
    CX_METHOD_RUN(this->Reset, this);
}

void cxActionResume(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->pauseTime = 0.0f;
    this->isPause = false;
}

void cxActionStop(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->isExit = true;
}

cxUInt cxActionGetId(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    return this->actionId != 0 ? this->actionId : (cxUInt)pav;
}

void cxActionSetDelay(cxAny pav,cxFloat delay)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->delay = delay;
}

void cxActionSetId(cxAny pav,cxUInt actionId)
{
    CX_ASSERT_THIS(pav, cxAction);
    this->actionId = actionId;
}


