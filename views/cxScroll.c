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

static cxFloat cxScrollMoveCurve(cxAny pav,cxFloat time)
{
    time = time - 1.0f;
    return (powf(time, 3.0f) + 1.0f);
}

static void cxBodyResetScaleUpdate(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxAction);
    CX_ASSERT_VALUE(cxActionGetView(this), cxView, body);
    CX_ASSERT_VALUE(cxViewGetParentView(body), cxScroll, scroll);
    cxScrollUpdateBox(scroll);
    cxVec2f npos = cxViewGetPosition(body);
    cxScrollCheckPos(scroll, &npos);
    cxViewSetPosition(body, npos);
}

static void cxScrollResetScale(cxScroll this)
{
    cxView body = cxScrollGetBody(this);
    cxVec2f scale = cxViewGetScale(body);
    if(scale.x > this->range.max || scale.y > this->range.max){
        cxScale scale = cxScaleCreate(this->scaleTime, cxVec2fx(this->range.max));
        cxActionSetCurve(scale, cxScrollMoveCurve);
        CX_ADD(cxAction, scale, onUpdate, cxBodyResetScaleUpdate);
        cxViewAppendAction(body, scale);
    }
}

static cxBool cxScrollScale(cxAny pview,const cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, cxScroll);
    cxTouchItem item1 = points->items[0];
    cxTouchItem item2 = points->items[1];
    cxView body = cxScrollGetBody(this);
    cxHitInfo h1 = cxViewHitTest(body, item1->position);
    cxHitInfo h2 = cxViewHitTest(body, item2->position);
    if(h1.hited && h2.hited && (item1->type == cxTouchTypeDown || item2->type == cxTouchTypeDown)){
        //disable move
        cxViewStopAction(body, (cxUInt)body);
        this->isEnable = false;
        this->startDis = kmVec2DistanceBetween(&item1->position, &item2->position);
        return true;
    }
    if(item1->type == cxTouchTypeUp || item2->type == cxTouchTypeUp){
        cxScrollResetScale(this);
        this->startDis = 0;
        return false;
    }
    if(this->startDis == 0){
        return false;
    }
    if(!h1.hited || !h2.hited){
        return false;
    }
    if(item1->type == cxTouchTypeMove || item2->type == cxTouchTypeMove){
        cxFloat dis = kmVec2DistanceBetween(&item1->position, &item2->position);
        cxFloat delta = dis - this->startDis;
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
        cxVec2f npos = cxViewSetAnchor(body, cxVec2fMidPoint(h1.position, h2.position));
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
    if(this->type & cxScrollMoveTypeVertical){
        if(pos->y<this->box.b){
            pos->y=this->box.b;
            fix=true;
        }
        if(pos->y>this->box.t){
            pos->y=this->box.t;
            fix=true;
        }
    }
    if(this->type & cxScrollMoveTypeHorizontal){
        if(pos->x<this->box.l){
            pos->x=this->box.l;
            fix=true;
        }
        if(pos->x>this->box.r){
            pos->x=this->box.r;
            fix=true;
        }
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

CX_METHOD_DEF(cxScroll,OnTouch,cxBool,const cxTouchItems *points)
{
    cxView body = cxScrollGetBody(this);
    CX_RETURN(body == NULL,false);
    if(this->scalable && points->number == 2){
        return cxScrollScale(this, points);
    }
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    cxHitInfo hit = cxViewHitTest(this, item->position);
    if(item->type == cxTouchTypeDown){
        this->isEnable = hit.hited;
        cxViewStopAction(body, (cxUInt)body);
        cxScrollUpdateBox(this);
        return hit.hited;
    }
    if(!this->isEnable){
        return false;
    }
    if(item->type == cxTouchTypeMove){
        cxVec2f vpos = cxViewGetPosition(body);
        cxBool setPos = false;
        if(this->type & cxScrollMoveTypeHorizontal){
            vpos.x += item->delta.x;
            setPos = true;
        }
        if(this->type & cxScrollMoveTypeVertical){
            vpos.y += item->delta.y;
            setPos = true;
        }
        if(setPos){
            cxScrollCheckPos(this, &vpos);
            cxViewSetPosition(body, vpos);
        }
        return true;
    }
    if(item->type == cxTouchTypeUp && item->movement > 15){
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
        cxActionSetId(m, (cxUInt)body);
        cxActionSetCurve(m, cxScrollMoveCurve);
        cxViewAppendAction(body, m);
    }
    return false;
}

//pos body
void cxScrollLocationTo(cxAny pview,cxVec2f pos,cxBool animate)
{
    CX_ASSERT_THIS(pview, cxScroll);
    this->location = pos;
    cxView body = cxScrollGetBody(this);
    CX_ASSERT(body != NULL, "body error");
    cxScrollUpdateBox(this);
    cxHitInfo hit = cxViewHitTest(body, cxVec2fv(0, 0));
    if(!hit.hited){
        return;
    }
    cxVec2f xpos = cxVec2fSub(hit.position, this->location);
    cxVec2f scale = cxViewTransformScale(body);
    cxVec2f cpos = cxViewGetPosition(body);
    cpos = cxVec2fAdd(cpos, cxVec2fScale2f(xpos, scale));
    cxScrollCheckPos(this, &cpos);
    if(animate){
        cxMove m = cxMoveCreate(this->moveTime, cpos);
        cxActionSetId(m, (cxUInt)body);
        cxActionSetCurve(m, cxScrollMoveCurve);
        cxViewAppendAction(body, m);
    }else{
        cxViewSetPosition(body, cpos);
    }
}
CX_SETTER_DEF(cxScroll, index)
{
    cxScrollSetBodyIdx(this, cxJsonToInt(value, this->BodyIdx));
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
    //init body position
    if(cxConstCharsHas(type, "left")){
        this->type |= cxScrollMoveTypeLeft;
    }
    if(cxConstCharsHas(type, "right")){
        this->type |= cxScrollMoveTypeRight;
    }
    if(cxConstCharsHas(type, "top")){
        this->type |= cxScrollMoveTypeTop;
    }
    if(cxConstCharsHas(type, "bottom")){
        this->type |= cxScrollMoveTypeBottom;
    }
}
CX_METHOD_DEF(cxScroll, OnEnter, void)
{
    cxScrollUpdate(this);
}
CX_TYPE(cxScroll, cxView)
{
    CX_SETTER(cxScroll, scaling);
    CX_SETTER(cxScroll, range);
    CX_SETTER(cxScroll, movetime);
    CX_SETTER(cxScroll, scaletime);
    CX_SETTER(cxScroll, scalable);
    CX_SETTER(cxScroll, scaleinc);
    CX_SETTER(cxScroll, layout);
    CX_SETTER(cxScroll, index);
    
    CX_METHOD(cxScroll, OnTouch);
    CX_METHOD(cxScroll, OnEnter);
}
CX_INIT(cxScroll, cxView)
{
    this->scaleinc = 0.5f;
    this->speed = 0.1f;
    this->scaling = 0.05f;
    this->range = cxRange2fv(0.5f, 1.5f);
    this->moveTime = 1.0f;
    this->scaleTime = 0.5f;
    this->scalable = true;
    this->type = cxScrollMoveTypeVertical|cxScrollMoveTypeHorizontal;
}
CX_FREE(cxScroll, cxView)
{
    
}
CX_TERM(cxScroll, cxView)

void cxScrollUpdate(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxScroll);
    cxScrollUpdateBox(this);
    cxView body = cxScrollGetBody(this);
    CX_ASSERT(body != NULL, "cxScroll not set body view");
    cxVec2f pos = cxViewGetPosition(body);
    if(this->type & cxScrollMoveTypeRight){
        pos.x = this->box.r;
    }
    if(this->type & cxScrollMoveTypeLeft){
        pos.x = this->box.l;
    }
    if(this->type & cxScrollMoveTypeTop){
        pos.y = this->box.t;
    }
    if(this->type & cxScrollMoveTypeBottom){
        pos.y = this->box.b;
    }
    cxViewSetPosition(body, pos);
}








