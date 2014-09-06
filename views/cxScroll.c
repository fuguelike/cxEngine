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
#include <actions/cxScale.h>
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

static cxFloat cxScrollMoveCurve(cxAny pav,cxFloat time)
{
    time = time - 1.0f;
    return (powf(time, 3.0f) + 1.0f);
}

static void cxScrollResetScale(cxScroll this)
{
    cxView body = cxScrollBody(this);
    if(body->scale.x > this->range.max || body->scale.y > this->range.max){
        cxScale scale = cxScaleCreate(this->scaleTime, cxVec2fx(this->range.max));
        cxActionSetCurve(scale, cxScrollMoveCurve);
        cxViewAppendAction(body, scale);
    }
}

static cxBool cxScrollScale(cxAny pview,cxTouchItems *points)
{
    cxScroll this = pview;
    cxVec2f cp0;
    cxVec2f cp1;
    cxTouchItem item0 = points->items[0];
    cxTouchItem item1 = points->items[1];
    cxView body = cxScrollBody(this);
    bool hited = cxViewHitTest(body, item0->position, &cp0) && cxViewHitTest(body, item1->position, &cp1);
    if(hited && item0->type == cxTouchTypeDown && item1->type == cxTouchTypeDown){
        //disable move
        this->isEnable = false;
        this->startDis = kmVec2DistanceBetween(&item0->position, &item1->position);
        kmVec2MidPointBetween(&this->center, &cp0, &cp1);
        return true;
    }
    if(item0->type == cxTouchTypeUp || item1->type == cxTouchTypeUp){
        cxScrollResetScale(this);
        this->startDis = 0;
    }
    if(this->startDis == 0){
        return false;
    }
    if(item0->type == cxTouchTypeMove || item1->type == cxTouchTypeMove){
        cxFloat dis = kmVec2DistanceBetween(&item0->position, &item1->position);
        cxFloat delta = (dis - this->startDis);
        this->startDis = dis;
        cxVec2f scale = body->scale;
        if(delta > 0){
            scale.x += this->scaling;
            scale.y += this->scaling;
        }else if(delta < 0){
            scale.x -= this->scaling;
            scale.y -= this->scaling;
        }else{
            return false;
        }
        scale.x = kmClamp(scale.x, this->range.min, this->range.max + this->scaleinc);
        scale.y = kmClamp(scale.y, this->range.min, this->range.max + this->scaleinc);
        cxViewSetScale(body, scale);
        //fix position
        cxVec2f npos = cxViewSetAnchor(body, this->center);
        cxScrollUpdateBox(this);
        cxScrollCheckPos(this, &npos);
        cxViewSetPos(body, npos);
        return true;
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

cxBool cxScrollTouch(cxAny pview,cxTouchItems *points)
{
    cxScroll this = pview;
    CX_RETURN(this->body == NULL,false);
    if(this->scalable && points->number == 2){
        return cxScrollScale(pview, points);
    }
    if(points->number != 1){
        return false;
    }
    cxView body = cxScrollBody(this);
    cxTouchItem item = points->items[0];
    cxVec2f cpos;
    cxBool hited = cxViewHitTest(this, item->position, &cpos);
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
        cxBool setPos = false;
        if(this->type & cxScrollMoveTypeHorizontal){
            vpos.x += delta.x;
            setPos = true;
        }
        if(this->type & cxScrollMoveTypeVertical){
            vpos.y += delta.y;
            setPos = true;
        }
        if(setPos){
            cxScrollCheckPos(pview, &vpos);
            cxViewSetPos(body, vpos);
        }
        return true;
    }
    if(item->type == cxTouchTypeUp){
        this->isEnable = false;
        if(fabsf(item->speed.y) < this->limit && fabsf(item->speed.x) < this->limit){
            return false;
        }
        cxVec2f npos = cxViewPosition(body);
        if(this->type & cxScrollMoveTypeVertical){
            npos.y += (item->speed.y / this->limit) * this->cxView.size.h * this->speed;
        }
        if(this->type & cxScrollMoveTypeHorizontal){
            npos.x += (item->speed.x / this->limit) * this->cxView.size.w * this->speed;
        }
        cxScrollCheckPos(this, &npos);
        cxMove m = cxMoveCreate(this->moveTime, npos);
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
CX_SETTER_DEF(cxScroll, movetime)
{
    this->moveTime = cxJsonToDouble(value, this->moveTime);
}
CX_SETTER_DEF(cxScroll, scaletime)
{
    this->scaleTime = cxJsonToDouble(value, this->scaleTime);
}
CX_SETTER_DEF(cxScroll, scaleinc)
{
    this->scaleinc = cxJsonToDouble(value, this->scaleinc);
}
CX_SETTER_DEF(cxScroll, body)
{
    cxObjectCreateResult ret = cxObjectCreateBegin(value);
    CX_ASSERT(CX_INSTANCE_OF(ret.object, cxView), "body must is cxView type");
    cxScrollSetBody(this, ret.object);
    cxObjectCreateEnd(&ret);
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
    CX_PROPERTY_SETTER(cxScroll, movetime);
    CX_PROPERTY_SETTER(cxScroll, scaletime);
    CX_PROPERTY_SETTER(cxScroll, scalable);
    CX_PROPERTY_SETTER(cxScroll, scaleinc);
    CX_PROPERTY_SETTER(cxScroll, body);
    CX_PROPERTY_SETTER(cxScroll, layout);
}
CX_OBJECT_INIT(cxScroll, cxView)
{
    this->scaleinc = 0.3f;
    this->limit = 1200;
    this->speed = 0.1f;
    this->scaling = 0.05f;
    this->range = cxRange2fv(0.5f, 1.5f);
    this->moveTime = 1.0f;
    this->scaleTime = 0.5f;
    this->scalable = true;
    CX_METHOD_SET(this->cxView.Touch, cxScrollTouch);
    cxViewSetCropping(this, true);
    this->type = cxScrollMoveTypeVertical|cxScrollMoveTypeHorizontal;
}
CX_OBJECT_FREE(cxScroll, cxView)
{
    
}
CX_OBJECT_TERM(cxScroll, cxView)











