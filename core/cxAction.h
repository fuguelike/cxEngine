//
//  cxAction.h
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAction_h
#define cxEngine_cxAction_h

#include <actions/cxCurve.h>
#include "cxBase.h"
#include "cxView.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxAction, cxObject)
    cxAssist4f assist;
    cxUInt actionId;
    cxFloat speed;
    cxFloat delay;
    cxFloat prevTime;
    cxFloat delta;
    cxFloat delayElapsed;
    cxFloat duration;
    cxFloat durationElapsed;
    cxBool stepHide;        //at view hide step action default false
    cxBool isExit;
    cxBool isPause;
    cxBool isFirst;
    cxBool isActive;
    cxView view;
    cxInt split;
    cxInt index;
    cxFloat splitDelta;
    CX_METHOD_ALLOC(Curve);
    CX_METHOD_ALLOC(Init);
    CX_METHOD_ALLOC(Active);
    CX_METHOD_ALLOC(Over);
    CX_METHOD_ALLOC(Reset);
    CX_METHOD_ALLOC(Step);
    CX_METHOD_ALLOC(Exit);
    CX_EVENT_ALLOC(onStart);
    CX_EVENT_ALLOC(onStop);
    CX_EVENT_ALLOC(onSplit);
    CX_EVENT_ALLOC(onStep);
CX_OBJECT_END(cxAction)

cxAny cxActionView(cxAny pav);

void cxActionSetStepHide(cxAny pav,cxBool stepHide);

void cxActionSetSplit(cxAny pav,cxInt split);

cxBool cxActionForever(cxAny pav);

void cxActionReadAttr(cxReaderAttrInfo *info);

void cxActionSetDuration(cxAny pav,cxFloat time);

void cxActionSetSpeed(cxAny pav,cxFloat speed);

void cxActionSetCurve(cxAny pav,cxActionCurveFunc curve);

cxUInt cxActionGetId(cxAny pav);

void cxActionSetId(cxAny pav,cxUInt actionId);

void cxActionSetDuration(cxAny pav,cxFloat duration);

void cxActionSetDelay(cxAny pav,cxFloat delay);

cxBool cxActionUpdate(cxAny pav,cxFloat dt);

void cxActionReset(cxAny pav);

void cxActionStop(cxAny pav);

void cxActionPause(cxAny pav);

void cxActionResume(cxAny pav);

CX_C_END

#endif
