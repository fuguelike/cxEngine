//
//  cxScroll.c
//  cxEngine
//
//  Created by xuhua on 11/5/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <actions/cxMove.h>
#include <actions/cxSpeed.h>
#include "cxScroll.h"

#define CX_SCROLL_MOVE_ACTION_ID    100000

void cxScrollSetBody(cxAny pview,cxAny cview)
{
    cxScroll this = pview;
    CX_ASSERT(this->body == NULL, "container already set");
    cxViewAppend(this, cview);
    this->body = cview;
}

cxView cxScrollBody(cxAny pview)
{
    cxScroll this = pview;
    return this->body;
}

static cxBool cxScrollScale(cxAny pview,cxTouchItems *fires,cxTouchItems *points)
{
    return false;
}

static cxBool cxScrollCheckPos(cxScroll this,cxVec2f *pos)
{
    cxBool fix = false;
    if(pos->y < this->box.b){
        pos->y = this->box.b;
        fix = true;
    }
    if(pos->y > this->box.t){
        pos->y = this->box.t;
        fix = true;
    }
    if(pos->x < this->box.l){
        pos->x = this->box.l;
        fix = true;
    }
    if(pos->x > this->box.r){
        pos->x = this->box.r;
        fix = true;
    }
    return fix;
}

static cxBool cxScrollUseValue(cxAny pav,cxVec2f pos)
{
    cxAny body = cxActionView(pav);
    CX_ASSERT(body != NULL, "body null");
    cxVec2f opos = cxViewPosition(body);
    cxScroll this = cxViewParent(body);
    CX_ASSERT(this != NULL, "scroll null");
    cxBool fix = cxScrollCheckPos(this, &pos);
    if(this->type & cxScrollMoveTypeHorizontal){
        opos.x = pos.x;
    }
    if(this->type & cxScrollMoveTypeVertical){
        opos.y = pos.y;
    }
    cxViewSetPos(body, opos);
    return fix;
}

cxBool cxScrollTouch(cxAny pview,cxTouchItems *fires,cxTouchItems *points)
{
    cxScroll this = pview;
    CX_RETURN(this->body == NULL,false);
    if(this->scalable && CX_TOUCH_SIZE(points) == 2){
        return cxScrollScale(pview, fires, points);
    }
    if(CX_TOUCH_SIZE(points) != 1){
        return false;
    }
    cxView body = cxScrollBody(this);
    cxTouchItem item = CX_TOUCH_ITEM(fires, 0);
    cxVec2f cpos;
    cxBool hited = cxViewHitTest(this, item->current, &cpos);
    if(item->type == cxTouchTypeDown){
        this->isEnable = hited;
        cxViewStopAction(body, CX_SCROLL_MOVE_ACTION_ID);
        cxSize2f msize = cxViewContentSize(body);
        cxSize2f psize = cxViewSize(this);
        cxFloat mw = (msize.w - psize.w) / 2.0f;
        cxVec2f anchor = cxViewAnchor(body);
        this->box.l = -mw + anchor.x * msize.w;
        this->box.r =  mw + anchor.x * msize.w;
        cxFloat mh = (msize.h - psize.h) / 2.0f;
        this->box.b = -mh + anchor.y * msize.h;
        this->box.t =  mh + anchor.y * msize.h;
        return hited;
    }
    if(!this->isEnable){
        return false;
    }
    if(item->type == cxTouchTypeMove){
        cxVec2f vpos = cxViewPosition(body);
        cxVec2f delta = cxViewTouchDelta(body, item);
        if(this->type & cxScrollMoveTypeHorizontal){
            vpos.x += delta.x;
        }
        if(this->type & cxScrollMoveTypeVertical){
            vpos.y += delta.y;
        }
        cxScrollCheckPos(pview, &vpos);
        cxViewSetPos(body, vpos);
        return true;
    }
    if(item->type == cxTouchTypeUp){
        cxVec2f aspeed = cxVec2fv(0, 0);
        CX_LOGGER("speed:%f %f",item->speed.x,item->speed.y);
        if(this->type & cxScrollMoveTypeHorizontal){
            aspeed.x = -item->speed.x / (this->time * this->time);
        }
        if(this->type & cxScrollMoveTypeVertical){
            aspeed.y = -item->speed.y / (this->time * this->time);
        }
        if(!cxVec2fZero(aspeed)){
            cxSpeed a = cxSpeedCreate(item->speed, aspeed);
            cxActionSetId(a, CX_SCROLL_MOVE_ACTION_ID);
            cxActionSetTime(a, this->time);
            CX_METHOD_SET(a->UseValue, cxScrollUseValue);
            cxViewAppendAction(body, a);
        }
        this->isEnable = false;
    }
    return false;
}

CX_SETTER_DEF(cxScroll, body)
{
    cxAny body = cxObjectLoadWithJson(value);
    CX_ASSERT(CX_INSTANCE_OF(body, cxView), "body must is cxView type");
    cxScrollSetBody(this, body);
}
CX_SETTER_DEF(cxScroll, scalable)
{
    this->scalable = cxJsonToBool(value, this->scalable);
}
CX_SETTER_DEF(cxScroll, layout)
{
    cxConstChars type = cxJsonToConstChars(value);
    this->type = type != NULL ? cxScrollMoveTypeNone : this->type;
    if(cxConstCharsHas(type, "horizontal")){
        this->type |= cxScrollMoveTypeHorizontal;
    }
    if(cxConstCharsHas(type, "vertical")){
        this->type |= cxScrollMoveTypeVertical;
    }
}

CX_OBJECT_TYPE(cxScroll, cxView)
{
    CX_PROPERTY_SETTER(cxScroll, scalable);
    CX_PROPERTY_SETTER(cxScroll, body);
    CX_PROPERTY_SETTER(cxScroll, layout);
}
CX_OBJECT_INIT(cxScroll, cxView)
{
    this->time = 0.3f;
    this->scalable = true;
    CX_METHOD_SET(this->cxView.Touch, cxScrollTouch);
    cxViewSetCropping(this, true);
    this->type = cxScrollMoveTypeVertical;
}
CX_OBJECT_FREE(cxScroll, cxView)
{
    
}
CX_OBJECT_TERM(cxScroll, cxView)











