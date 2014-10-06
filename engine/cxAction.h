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
#include "cxGroup.h"

CX_C_BEGIN

#define CX_FOREVER   -1

CX_OBJECT_DEF(cxAction, cxObject)
    CX_FIELD_DEF(cxAny Parent);
    CX_FIELD_DEF(cxAssist4f Assist);
    cxBool isExit;
    cxBool isPause;
    cxBool isFirst;
    cxBool isActive;
    CX_FIELD_DEF(cxFloat Scale);
    CX_FIELD_DEF(cxFloat InitTime);
    CX_FIELD_DEF(cxFloat Delay);
    CX_FIELD_DEF(cxFloat DelayElapsed);
    CX_FIELD_DEF(cxFloat Time);
    CX_FIELD_DEF(cxFloat TimeElapsed);
    CX_FIELD_DEF(cxUInt ActionId);
    CX_FIELD_DEF(cxLong Tag);
    CX_FIELD_DEF(cxFloat PauseTime);
    CX_FIELD_DEF(cxAny View);
    CX_FIELD_DEF(cxGroup Group);
    CX_METHOD_DEF(cxFloat, Curve, cxAny, cxFloat);
    CX_METHOD_DEF(void, Init, cxAny);
    CX_METHOD_DEF(void, Active, cxAny);
    CX_METHOD_DEF(void, Over, cxAny);
    CX_METHOD_DEF(void, Reset, cxAny);
    CX_METHOD_DEF(void, Step,cxAny, cxFloat, cxFloat);
    CX_METHOD_DEF(cxBool, Exit, cxAny);
    CX_EVENT_ALLOC(onInit);
    CX_EVENT_ALLOC(onExit);
    CX_EVENT_ALLOC(onUpdate);
CX_OBJECT_END(cxAction, cxObject)

CX_FIELD_SET(cxAction, cxFloat, PauseTime);
CX_FIELD_SET(cxAction, cxFloat, InitTime);
CX_FIELD_IMP(cxAction, cxFloat, Time);
CX_FIELD_IMP(cxAction, cxAny, Parent);
CX_FIELD_IMP(cxAction, cxAssist4f, Assist);
CX_FIELD_IMP(cxAction, cxFloat, Scale);
CX_FIELD_GET(cxAction, cxFloat, TimeElapsed);
CX_FIELD_GET(cxAction, cxGroup, Group);
CX_FIELD_IMP(cxAction, cxFloat, Delay);
CX_FIELD_GET(cxAction, cxFloat, DelayElapsed);
CX_FIELD_IMP(cxAction, cxAny, View);
CX_FIELD_IMP(cxAction, cxFloat, Tag);

CX_FIELD_SET(cxAction, cxUInt, ActionId);
CX_INLINE cxUInt cxActionGetActionId(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxAction);
    return this->ActionId == 0 ? (cxUInt)this:this->ActionId;
}

void cxActionSetGroup(cxAny pav,cxConstChars name);

cxBool cxActionForever(cxAny pav);

void cxActionSetCurve(cxAny pav,cxActionCurveFunc curve);

cxBool cxActionUpdate(cxAny pav,cxFloat dt);

void cxActionReset(cxAny pav);

void cxActionStop(cxAny pav);

void cxActionPause(cxAny pav);

void cxActionResume(cxAny pav);

CX_C_END

#endif
