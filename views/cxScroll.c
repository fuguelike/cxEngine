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
    cxScroll this = pview;
    cxVec2f cp0;
    cxVec2f cp1;
    cxTouchItem item0 = CX_TOUCH_ITEM(points, 0);
    cxTouchItem item1 = CX_TOUCH_ITEM(points, 1);
    bool hited = cxViewHitTest(this->body, item0->current, &cp0) && cxViewHitTest(this->body, item1->current, &cp1);
    if(hited && item0->type == cxTouchTypeDown && item1->type == cxTouchTypeDown){
        //disable move
        this->isEnable = false;
        this->startDis = kmVec2DistanceBetween(&item0->current, &item1->current);
        kmVec2MidPointBetween(&this->center, &cp0, &cp1);
        return true;
    }
    if(this->startDis == 0){
        return false;
    }
    if(item0->type == cxTouchTypeMove || item1->type == cxTouchTypeMove){
        cxFloat dis = kmVec2DistanceBetween(&item0->current, &item1->current);
        cxFloat delta = (dis - this->startDis);
        this->startDis = dis;
        if(fabsf(delta) < 2 || delta == 0){
            return false;
        }
        cxVec2f scale = cxViewScale(this->body);
        if(delta > 0){
            scale.x += this->scaling;
            scale.y += this->scaling;
        }else if(delta < 0){
            scale.x -= this->scaling;
            scale.y -= this->scaling;
        }
        scale.x = kmClamp(scale.x, this->range.min, this->range.max);
        scale.y = kmClamp(scale.y, this->range.min, this->range.max);
        cxViewSetScale(this->body, scale);
        //fix position
        cxVec2f npos = cxViewSetAnchor(this->body, this->center);
        cxScrollUpdateBox(this);
        cxScrollCheckPos(this, &npos);
        cxViewSetPos(this->body, npos);
        return true;
    }
    if(item0->type == cxTouchTypeUp || item1->type == cxTouchTypeUp){
        this->startDis = 0;
    }
    return false;
}

cxBool cxScrollCheckPos(cxScroll this,cxVec2f *pos)
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

void cxScrollUpdateBox(cxAny pview)
{
    cxScroll this = pview;
    cxView body = cxScrollBody(this);
    cxSize2f msize = cxViewContentSize(body);
    cxSize2f psize = cxViewSize(this);
    cxFloat mw = (msize.w - psize.w) / 2.0f;
    cxVec2f anchor = cxViewAnchor(body);
    this->box.l = -mw + anchor.x * msize.w;
    this->box.r =  mw + anchor.x * msize.w;
    cxFloat mh = (msize.h - psize.h) / 2.0f;
    this->box.b = -mh + anchor.y * msize.h;
    this->box.t =  mh + anchor.y * msize.h;
}

static cxFloat cxScrollMoveCurve(cxAny pav,cxFloat time)
{
    time = time - 1.0f;
    return (powf(time, 3.0f) + 1.0f);
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
        cxScrollUpdateBox(this);
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
        this->isEnable = false;
        if(fabsf(item->speed.y) < this->limit && fabsf(item->speed.x) < this->limit){
            return false;
        }
        cxVec2f new = cxViewPosition(this->body);
        if(this->type & cxScrollMoveTypeVertical){
            new.y += (item->speed.y / this->limit) * this->cxView.size.h * this->speed;
        }
        if(this->type & cxScrollMoveTypeHorizontal){
            new.x += (item->speed.x / this->limit) * this->cxView.size.w * this->speed;
        }
        cxScrollCheckPos(this, &new);
        cxMove m = cxMoveCreate(this->time, new);
        cxActionSetId(m, CX_SCROLL_MOVE_ACTION_ID);
        cxActionSetCurve(m, cxScrollMoveCurve);
        cxViewAppendAction(body, m);
    }
    return false;
}

CX_SETTER_DEF(cxScroll, range)
{
    this->range = cxJsonToRange2f(value, this->range);
}
CX_SETTER_DEF(cxScroll, scaling)
{
    this->scaling = cxJsonToDouble(value, this->scaling);
}
CX_SETTER_DEF(cxScroll, time)
{
    this->time = cxJsonToDouble(value, this->time);
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
    CX_PROPERTY_SETTER(cxScroll, scaling);
    CX_PROPERTY_SETTER(cxScroll, range);
    CX_PROPERTY_SETTER(cxScroll, time);
    CX_PROPERTY_SETTER(cxScroll, scalable);
    CX_PROPERTY_SETTER(cxScroll, body);
    CX_PROPERTY_SETTER(cxScroll, layout);
}
CX_OBJECT_INIT(cxScroll, cxView)
{
    this->limit = 1200;
    this->speed = 0.1f;
    this->scaling = 0.05f;
    this->range = cxRange2fv(0.5f, 1.5f);
    this->time = 1.0f;
    this->scalable = true;
    CX_METHOD_SET(this->cxView.Touch, cxScrollTouch);
    cxViewSetCropping(this, true);
    this->type = cxScrollMoveTypeVertical|cxScrollMoveTypeHorizontal;
}
CX_OBJECT_FREE(cxScroll, cxView)
{
    
}
CX_OBJECT_TERM(cxScroll, cxView)











