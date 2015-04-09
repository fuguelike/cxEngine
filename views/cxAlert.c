//
//  cxAlert.c
//  cxCore
//
//  Created by xuhua on 6/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <actions/cxScale.h>
#include "cxWindow.h"
#include "cxAlert.h"

CX_METHOD_DEF(cxAlert,OnTouch,cxBool,const cxTouchItems *points)
{
    cxTouchItem item = points->items[0];
    cxHitInfo info = cxViewHitTest(this, item->position);
    if(item->type == cxTouchTypeUp && !info.hited){
        cxAlertHide(this);
    }
    return true;
}
CX_METHOD_DEF(cxAlert,OnKey,cxBool,const cxKey *key)
{
    if(key->type == cxKeyTypeUp && key->code == CX_KEYCODE_BACK){
        cxAlertHide(this);
        return true;
    }
    return false;
}

//when alert show
CX_METHOD_DEF(cxAlert, OnShow,void)
{

}
//when alert hide
CX_METHOD_DEF(cxAlert, OnHide,void)
{

}
static void cxShowActionOnStop(cxAny pav)
{
    CX_ASSERT_VALUE(cxActionGetView(pav), cxAlert, this);
    CX_CALL(this, OnShow, CX_M(void));
}
CX_METHOD_DEF(cxAlert, HowShow, void)
{
    cxViewSetScale(this, cxVec2fx(0.9f));
    cxScale m = cxScaleCreate(0.2f, cxVec2fx(1.0f));
    CX_ADD(cxAction, m, onExit, cxShowActionOnStop);
    cxActionSetCurve(m, cxCurveBackOut);
    cxViewAppendAction(this, m);
    cxWindowPushView(this);
}
static void cxHideActionOnStop(cxAny pav)
{
    CX_ASSERT_VALUE(cxActionGetView(pav), cxAlert, this);
    CX_CALL(this, OnHide, CX_M(void));
    cxWindowPopView();
}
CX_METHOD_DEF(cxAlert, HowHide, void)
{
    cxScale m = cxScaleCreate(0.2f, cxVec2fx(0.9f));
    CX_ADD(cxAction, m, onExit, cxHideActionOnStop);
    cxActionSetCurve(m, cxCurveBackIn);
    cxViewAppendAction(this, m);
}
CX_TYPE(cxAlert, cxAtlas)
{
    CX_METHOD(cxAlert, OnTouch);
    CX_METHOD(cxAlert, OnKey);
    CX_METHOD(cxAlert, OnShow);
    CX_METHOD(cxAlert, OnHide);
    CX_METHOD(cxAlert, HowShow);
    CX_METHOD(cxAlert, HowHide);
}
CX_INIT(cxAlert, cxAtlas)
{
    cxViewSetSleepTop(this, false);
}
CX_FREE(cxAlert, cxAtlas)
{
    
}
CX_TERM(cxAlert, cxAtlas)

void cxAlertShow(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAlert);
    CX_CALL(this, HowShow, CX_M(void));
}

void cxAlertHide(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxAlert);
    CX_CALL(this, HowHide, CX_M(void));

}







