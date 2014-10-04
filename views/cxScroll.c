//
//  cxScroll.c
//  cxEngine
//
//  Created by xuhua on 11/5/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <actions/cxMove.h>
#include <actions/cxScale.h>
#include "cxScroll.h"

#define CX_SCROLL_MOVE_ACTION_ID    100000

void cxScrollSetBody(cxAny pview,cxAny body)
{
    CX_ASSERT_THIS(pview, cxScroll);
    CX_ASSERT(this->Body == NULL, "container already set");
    cxViewPrepend(this, body);
    this->Body = body;
}

static cxFloat cxScrollMoveCurve(cxAny pav,cxFloat time)
{
    time = time - 1.0f;
    return (powf(time, 3.0f) + 1.0f);
}

static void cxScrollResetScale(cxScroll this)
{
    cxView body = cxScrollGetBody(this);
    cxVec2f scale = cxViewGetScale(body);
    if(scale.x > this->range.max || scale.y > this->range.max){
        cxScale scale = cxScaleCreate(this->scaleTime, cxVec2fx(this->range.max));
        cxActionSetCurve(scale, cxScrollMoveCurve);
        cxViewAppendAction(body, scale);
    }
}

static cxBool cxScrollScale(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, cxScroll);
    cxVec2f cp0;
    cxVec2f cp1;
    cxTouchItem item0 = points->items[0];
    cxTouchItem item1 = points->items[1];
    cxView body = cxScrollGetBody(this);
    bool hited = cxViewHitTest(body, item0->position, &cp0) && cxViewHitTest(body, item1->position, &cp1);
    if(hited && (item0->type == cxTouchTypeDown || item1->type == cxTouchTypeDown)){
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
        cxVec2f scale = cxViewGetScale(body);
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
        cxViewSetPosition(body, npos);
        return true;
    }
    return false;
}

cxBool cxScrollCheckPos(cxAny pview,cxVec2f *pos)
{
    CX_ASSERT_THIS(pview, cxScroll);
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
    CX_ASSERT_THIS(pview, cxScroll);
    cxView body = cxScrollGetBody(this);
    cxSize2f msize = cxViewContentSize(body);
    cxSize2f psize = cxViewGetSize(this);
    cxFloat mw = (msize.w - psize.w) / 2.0f;
    cxVec2f anchor = cxViewGetAnchor(body);
    this->box.l = -mw + anchor.x * msize.w;
    this->box.r =  mw + anchor.x * msize.w;
    cxFloat mh = (msize.h - psize.h) / 2.0f;
    this->box.b = -mh + anchor.y * msize.h;
    this->box.t =  mh + anchor.y * msize.h;
}

cxBool cxScrollTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, cxScroll);
    CX_RETURN(this->Body == NULL,false);
    if(this->scalable && points->number == 2){
        return cxScrollScale(pview, points);
    }
    if(points->number != 1){
        return false;
    }
    cxView body = cxScrollGetBody(this);
    cxTouchItem item = points->items[0];
    cxVec2f cpos;
    cxBool hited = cxViewHitTest(this, item->position, &cpos);
    if(item->type == cxTouchTypeDown){
        this->isEnable = hited;
        this->movement = 0;
        cxViewStopAction(body, CX_SCROLL_MOVE_ACTION_ID);
        cxScrollUpdateBox(this);
        return hited;
    }
    if(!this->isEnable){
        return false;
    }
    if(item->type == cxTouchTypeMove){
        cxVec2f vpos = cxViewGetPosition(body);
        cxVec2f delta = cxViewTouchDelta(body, item);
        this->movement += cxVec2fLength(delta);
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
            cxViewSetPosition(body, vpos);
        }
        return true;
    }
    if(item->type == cxTouchTypeUp && this->movement > 0){
        this->isEnable = false;
        cxVec2f npos = cxViewGetPosition(body);
        if(this->type & cxScrollMoveTypeVertical){
            npos.y += item->speed.y * this->speed;
        }
        if(this->type & cxScrollMoveTypeHorizontal){
            npos.x += item->speed.x * this->speed;
        }
        cxScrollCheckPos(this, &npos);
        cxMove m = cxMoveCreate(this->moveTime, npos);
        cxActionSetActionId(m, CX_SCROLL_MOVE_ACTION_ID);
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
    cxAny object = cxObjectCreateUseJson(value);
    CX_ASSERT_TYPE(object, cxView);
    cxScrollSetBody(this, object);
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
    this->scaleinc = 0.5f;
    this->speed = 0.1f;
    this->scaling = 0.06f;
    this->range = cxRange2fv(0.5f, 1.5f);
    this->moveTime = 1.0f;
    this->scaleTime = 0.5f;
    this->scalable = true;
    CX_SET(cxView, this, Touch, cxScrollTouch);
    this->type = cxScrollMoveTypeVertical|cxScrollMoveTypeHorizontal;
}
CX_OBJECT_FREE(cxScroll, cxView)
{
    
}
CX_OBJECT_TERM(cxScroll, cxView)

void cxScrollSetEnable(cxAny pview,cxBool enable)
{
    CX_ASSERT_THIS(pview, cxScroll);
    this->isEnable = enable;
}










