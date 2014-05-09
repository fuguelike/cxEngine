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
    cxFloat durationInit;
    cxFloat duration;
    cxFloat durationElapsed;
    cxFloat pauseTime;
    cxBool isExit;
    cxBool isPause;
    cxBool isFirst;
    cxBool isActive;
    cxView view;
    cxInt indexNum;
    cxInt index;
    cxFloat indexDelta;
    CX_METHOD_DEF(cxFloat, Curve, cxAny, cxFloat);
    CX_METHOD_DEF(void, Init, cxAny);
    CX_METHOD_DEF(void, Active, cxAny);
    CX_METHOD_DEF(void, Over, cxAny);
    CX_METHOD_DEF(void, Reset, cxAny);
    CX_METHOD_DEF(void, Step,cxAny, cxFloat, cxFloat);
    CX_METHOD_DEF(cxBool, Exit, cxAny);
    CX_EVENT_ALLOC(onStart);
    CX_EVENT_ALLOC(onStop);
    CX_EVENT_ALLOC(onIndex);
    CX_EVENT_ALLOC(onStep);
CX_OBJECT_END(cxAction)

void cxActionSetPauseTime(cxAny pav,cxFloat time);

cxAny cxActionView(cxAny pav);

void cxActionSetView(cxAny pav,cxAny pview);

void cxActionSetIndex(cxAny pav,cxInt indexNum);

cxBool cxActionForever(cxAny pav);

void cxActionSetDuration(cxAny pav,cxFloat time);

void cxActionSetDurationInit(cxAny pav,cxFloat time);

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
