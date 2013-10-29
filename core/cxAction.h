//
//  cxAction.h
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAction_h
#define cxEngine_cxAction_h

#include "cxBase.h"
#include "cxView.h"
#include "cxCurve.h"

CX_C_BEGIN

typedef void (*cxActionStepFunc)(cxAny pav,cxFloat dt,cxFloat time);

typedef cxBool (*cxActionIsExitFunc)(cxAny pav);

typedef void (*cxActionFunc)(cxAny pav);

CX_OBJECT_DEF(cxAction, cxObject)
    cxAssist4f assist;
    cxUInt actionId;
    cxFloat scale;
    cxFloat delay;
    cxFloat prevTime;
    cxFloat delta;
    cxFloat delayElapsed;
    cxFloat duration;
    cxFloat durationElapsed;
    cxBool isExit;
    cxBool isPause;
    cxBool isFirst;
    cxBool isActive;
    cxView view;
    cxInt split;
    cxInt index;
    cxFloat splitDelta;
    CX_METHOD_DEF(cxActionCurveFunc,    Curve);
    CX_METHOD_DEF(cxActionFunc,         Init);
    CX_METHOD_DEF(cxActionFunc,         Active);
    CX_METHOD_DEF(cxActionFunc,         Over);
    CX_METHOD_DEF(cxActionStepFunc,     Step);
    CX_METHOD_DEF(cxActionIsExitFunc,   Exit);
    CX_EVENT_ALLOC(onStart);
    CX_EVENT_ALLOC(onStop);
    CX_EVENT_ALLOC(onSplit);
CX_OBJECT_END(cxAction)

void cxActionSetSplit(cxAny pav,cxInt split);

cxBool cxActionForever(cxAny pav);

cxBool cxActionXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader);

void cxActionSetDuration(cxAny pav,cxFloat time);

void cxActionSetScale(cxAny pav,cxFloat scale);

void cxActionSetCurve(cxAny pav,cxActionCurveFunc curve);

cxUInt cxActionGetId(cxAny pav);

void cxActionSetId(cxAny pav,cxUInt actionId);

void cxActionSetDuration(cxAny pav,cxFloat duration);

void cxActionSetDelay(cxAny pav,cxFloat delay);

cxBool cxActionUpdate(cxAny pav,cxFloat dt);

void cxActionStop(cxAny pav);

void cxActionPause(cxAny pav);

void cxActionResume(cxAny pav);

CX_C_END

#endif
