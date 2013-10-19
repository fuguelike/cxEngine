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

typedef cxAny (*cxActionCloneFunc)(cxAny pav);

CX_OBJECT_DEF(cxAction, cxObject)
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
    cxView view;
    CX_METHOD_DEF(cxActionCurveFunc,    Curve);
    CX_METHOD_DEF(cxActionFunc,         Init);
    CX_METHOD_DEF(cxActionFunc,         Over);
    CX_METHOD_DEF(cxActionStepFunc,     Step);
    CX_METHOD_DEF(cxActionIsExitFunc,   Exit);
    CX_METHOD_DEF(cxActionCloneFunc,    Clone);
    CX_EVENT_ALLOC(onBefore);
    CX_EVENT_ALLOC(onAfter);
CX_OBJECT_END(cxAction)

cxBool cxActionForever(cxAny pav);

void cxActionXMLReadAttr(cxAny xmlAction,cxAny mAction, xmlTextReaderPtr reader);

void cxActionSetDuration(cxAny pav,cxFloat time);

cxAny cxActionClone(cxAny pav);

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
