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

CX_DEF(cxAction, cxObject)
    cxBool isExit;
    cxBool isPause;
    cxBool isFirst;
    CX_FIELD_DEF(cxAny Parent);
    CX_FIELD_DEF(cxAssist4f Assist);
    CX_FIELD_DEF(cxFloat Scale);
    CX_FIELD_DEF(cxFloat InitTime);
    CX_FIELD_DEF(cxFloat Delay);
    CX_FIELD_DEF(cxFloat DelayElapsed);
    CX_FIELD_DEF(cxFloat Time);
    CX_FIELD_DEF(cxFloat TimeElapsed);
    CX_FIELD_DEF(cxUInt Id);
    CX_FIELD_DEF(cxLong Tag);
    CX_FIELD_DEF(cxAny View);
    CX_FIELD_DEF(cxGroup Group);
    CX_FIELD_DEF(cxActionCurveFunc Curve);
    CX_EVENT_ALLOC(onInit);
    CX_EVENT_ALLOC(onExit);
    CX_EVENT_ALLOC(onUpdate);
CX_END(cxAction, cxObject)

CX_FIELD_SET(cxAction, cxActionCurveFunc, Curve);
CX_FIELD_SET(cxAction, cxFloat, InitTime);

CX_FIELD_GET(cxAction, cxFloat, Time);
CX_INLINE void cxActionSetTime(cxAny pthis,const cxFloat time)
{
    CX_ASSERT_THIS(pthis, cxAction);
    this->Time = CX_MAX(time, FLT_EPSILON);
}

CX_FIELD_IMP(cxAction, cxAny, Parent);
CX_FIELD_IMP(cxAction, cxAssist4f, Assist);
CX_FIELD_IMP(cxAction, cxFloat, Scale);
CX_FIELD_GET(cxAction, cxFloat, TimeElapsed);
CX_FIELD_GET(cxAction, cxGroup, Group);
CX_FIELD_IMP(cxAction, cxFloat, Delay);
CX_FIELD_GET(cxAction, cxFloat, DelayElapsed);
CX_FIELD_IMP(cxAction, cxAny, View);
CX_FIELD_IMP(cxAction, cxFloat, Tag);

CX_FIELD_SET(cxAction, cxUInt, Id);
CX_INLINE cxUInt cxActionGetId(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxAction);
    return this->Id == 0 ? (cxUInt)this:this->Id;
}

void cxActionSetGroup(cxAny pav,cxConstChars name);

cxBool cxActionForever(cxAny pav);

cxBool cxActionUpdate(cxAny pav,cxFloat dt);

void cxActionStop(cxAny pav);

void cxActionPause(cxAny pav);

void cxActionResume(cxAny pav);

CX_C_END

#endif
