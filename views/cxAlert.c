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

static cxBool cxAlertKey(cxAny pview,cxKey *key)
{
    if(key->type == cxKeyTypeUp && key->code == CX_KEYCODE_BACK){
        cxAlertHide(pview);
        return true;
    }
    return false;
}

static cxBool cxAlertTouch(cxAny pview,cxTouchItems *points)
{
    return true;
}

static void cxShowActionOnStop(cxAny pav)
{
    CX_ASSERT_VALUE(cxActionGetView(pav), cxAlert, this);
    CX_EVENT_FIRE(this, OnShow);
}

static void cxAlertOnShow(cxAny pview)
{
    cxSize2f wsize = cxEngineWinSize();
    cxSize2f size = cxViewGetSize(pview);
    cxVec2f pos = cxViewGetPosition(pview);
    pos.y = -wsize.h/2.0f - size.h / 2.0f;
    cxViewSetPosition(pview, pos);
    cxMove m = cxMoveCreate(0.3f, cxVec2fv(pos.x, 0));
    CX_ADD(cxAction, m, onExit, cxShowActionOnStop);
    cxActionSetCurve(m, cxCurveBackOut);
    cxViewAppendAction(pview, m);
}

static void cxHideActionOnStop(cxAny pav)
{
    CX_ASSERT_VALUE(cxActionGetView(pav), cxAlert, this);
    CX_EVENT_FIRE(this, OnHide);
    cxWindowPopView();
}

static void cxAlertOnHide(cxAny pview)
{
    cxSize2f wsize = cxEngineWinSize();
    cxSize2f size = cxViewGetSize(pview);
    cxVec2f pos = cxViewGetPosition(pview);
    pos.y = -wsize.h/2.0f - size.h / 2.0f;
    cxMove m = cxMoveCreate(0.3f, pos);
    CX_ADD(cxAction, m, onExit, cxHideActionOnStop);
    cxActionSetCurve(m, cxCurveBackIn);
    cxViewAppendAction(pview, m);
}

CX_OBJECT_TYPE(cxAlert, cxAtlas)
{
    
}
CX_OBJECT_INIT(cxAlert, cxAtlas)
{
    cxViewSetHideTop(this, false);
    CX_SET(cxView, this, Key, cxAlertKey);
    CX_SET(cxView, this, Touch, cxAlertTouch);
    CX_SET(cxAlert, this, Hide, cxAlertOnHide);
    CX_SET(cxAlert, this, Show, cxAlertOnShow);
}
CX_OBJECT_FREE(cxAlert, cxAtlas)
{
    CX_EVENT_RELEASE(this->OnHide);
    CX_EVENT_RELEASE(this->OnShow);
}
CX_OBJECT_TERM(cxAlert, cxAtlas)

void cxAlertShow(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAlert);
    cxWindowPushView(this);
    CX_METHOD_RUN(this->Show,this);
}

void cxAlertHide(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAlert);
    CX_METHOD_RUN(this->Hide,this);
}







