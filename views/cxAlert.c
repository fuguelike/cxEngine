//
//  cxAlert.c
//  cxCore
//
//  Created by xuhua on 6/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <actions/cxMove.h>
#include "cxWindow.h"
#include "cxAlert.h"

static cxBool CX_METHOD(cxAlert,OnTouch,const cxTouchItems *points)
{
    return true;
}

static cxBool CX_METHOD(cxAlert,OnKey,const cxKey *key)
{
    if(key->type == cxKeyTypeUp && key->code == CX_KEYCODE_BACK){
        cxAlertFireHide(this);
        return true;
    }
    return false;
}

static void cxShowActionOnStop(cxAny pav)
{
    CX_ASSERT_VALUE(cxActionGetView(pav), cxAlert, this);
    CX_EVENT_FIRE(this, OnShow);
}


static void cxHideActionOnStop(cxAny pav)
{
    CX_ASSERT_VALUE(cxActionGetView(pav), cxAlert, this);
    CX_EVENT_FIRE(this, OnHide);
    cxWindowPopView();
}


static void CX_METHOD(cxAlert, Show)
{
    cxSize2f wsize = cxEngineGetWinSize();
    cxSize2f size = cxViewGetSize(this);
    cxVec2f pos = cxViewGetPosition(this);
    pos.y = -wsize.h/2.0f - size.h / 2.0f;
    cxViewSetPosition(this, pos);
    cxMove m = cxMoveCreate(0.3f, cxVec2fv(pos.x, 0));
    CX_ADD(cxAction, m, onExit, cxShowActionOnStop);
    cxActionSetCurve(m, cxCurveBackOut);
    cxViewAppendAction(this, m);
}
static void CX_METHOD(cxAlert, Hide)
{
    cxSize2f wsize = cxEngineGetWinSize();
    cxSize2f size = cxViewGetSize(this);
    cxVec2f pos = cxViewGetPosition(this);
    pos.y = -wsize.h/2.0f - size.h / 2.0f;
    cxMove m = cxMoveCreate(0.3f, pos);
    CX_ADD(cxAction, m, onExit, cxHideActionOnStop);
    cxActionSetCurve(m, cxCurveBackIn);
    cxViewAppendAction(this, m);
}

CX_TYPE(cxAlert, cxAtlas)
{
    CX_MSET(cxAlert, OnTouch);
    CX_MSET(cxAlert, OnKey);
    CX_MSET(cxAlert, Show);
    CX_MSET(cxAlert, Hide);
}
CX_INIT(cxAlert, cxAtlas)
{
    cxViewSetSleepTop(this, false);
}
CX_FREE(cxAlert, cxAtlas)
{
    CX_EVENT_RELEASE(this->OnHide);
    CX_EVENT_RELEASE(this->OnShow);
}
CX_TERM(cxAlert, cxAtlas)

void cxAlertFireShow(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAlert);
    cxWindowPushView(this);
    CX_CALL(this, Show, CX_MT(void));
}

void cxAlertFireHide(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAlert);
    CX_CALL(this, Hide, CX_MT(void));
}







