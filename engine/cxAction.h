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
#include <cxcore/cxBase.h>
#include "cxView.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxAction, cxObject)
    cxAny parent;
    cxAssist4f assist;
    cxUInt actionId;
    cxFloat scale;
    cxFloat delay;
    cxFloat delayElapsed;
    cxFloat timeInit;
    cxFloat time;
    cxFloat timeElapsed;
    cxFloat pauseTime;
    cxBool isExit;
    cxBool isPause;
    cxBool isFirst;
    cxBool isActive;
    cxView view;
    cxInt indexNum;
    cxInt index;
    cxFloat indexDelta;
    cxAny actionMgr;
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
    CX_EVENT_ALLOC(onUpdate);
CX_OBJECT_END(cxAction, cxObject)

void cxActionSetMgr(cxAny pav,cxAny mgr);

cxAny cxActionParent(cxAny pav);

void cxActionSetParent(cxAny pav,cxAny parent);

void cxActionSetPauseTime(cxAny pav,cxFloat time);

cxAny cxActionView(cxAny pav);

void cxActionSetView(cxAny pav,cxAny pview);

void cxActionSetIndex(cxAny pav,cxInt indexNum);

void cxActionSetScale(cxAny pav,cxFloat scale);

cxBool cxActionForever(cxAny pav);

void cxActionSetTime(cxAny pav,cxFloat time);

void cxActionSetTimeInit(cxAny pav,cxFloat time);

void cxActionSetCurve(cxAny pav,cxActionCurveFunc curve);

cxUInt cxActionGetId(cxAny pav);

void cxActionSetId(cxAny pav,cxUInt actionId);

void cxActionSetDelay(cxAny pav,cxFloat delay);

cxBool cxActionUpdate(cxAny pav,cxFloat dt);

void cxActionReset(cxAny pav);

void cxActionStop(cxAny pav);

void cxActionPause(cxAny pav);

void cxActionResume(cxAny pav);

CX_C_END

#endif
