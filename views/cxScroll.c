//
//  cxScroll.c
//  cxEngine
//
//  Created by xuhua on 11/5/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxViewRoot.h>
#include <core/cxEngine.h>
#include <actions/cxMove.h>
#include "cxScroll.h"

#define CX_SCROLL_MOVE_ACTION_ID 100000

void cxScrollReadAttr(cxReaderAttrInfo *info)
{
    cxViewReadAttr(info);
    cxScroll this = info->object;
    cxConstChars type = cxXMLAttr(info->reader,"cxScroll.type");
    if(cxConstCharsEqu(type, "horizontal")){
        this->type |= cxScrollMoveTypeHorizontal;
    }else if(cxConstCharsEqu(type, "vertical")){
        this->type |= cxScrollMoveTypeVertical;
    }else {
        this->type |= (cxScrollMoveTypeVertical|cxScrollMoveTypeHorizontal);
    }
    this->value = cxXMLReadFloatAttr(info, "cxScroll.value", this->value);
}

cxView cxScrollContainer(cxAny pview)
{
    cxScroll this = pview;
    cxListElement *ele = cxListFirst(this->super.subViews);
    CX_ASSERT(ele != NULL, "cxScroll not set conatiner,should first subviews");
    return ele->any;
}

static void cxScrollActionView(cxScroll this,cxVec2f new)
{
    cxView view = cxScrollContainer(this);
    cxFloat distance = kmVec2DistanceBetween(&new, &view->position);
    cxMove move = cxMoveCreate(distance/this->value, new);
    cxActionSetId(move, CX_SCROLL_MOVE_ACTION_ID);
    cxActionSetCurve(move, cxCurveCubeOut);
    cxViewAppendAction(view, move);
}

cxBool cxScrollTouch(cxAny pview,cxTouch *touch)
{
    cxScroll this = pview;
    cxView view = cxScrollContainer(this);
    if(!cxViewHitTest(pview, touch->current, NULL)){
        return false;
    }
    if(touch->type == cxTouchTypeMove){
        cxVec2f pos = cxViewPosition(view);
        cxBool setpos = false;
        if(this->type & cxScrollMoveTypeVertical){
            pos.y += touch->delta.y;
            setpos = true;
        }
        if(touch->delta.y < 0 && pos.y < this->box.b){
            pos.y = this->box.b;
        }
        if(touch->delta.y > 0 && pos.y > this->box.t){
            pos.y = this->box.t;
        }
        if(this->type & cxScrollMoveTypeHorizontal){
            pos.x += touch->delta.x;
            setpos = true;
        }
        if(touch->delta.x < 0 && pos.x < this->box.l){
            pos.x = this->box.l;
        }
        if(touch->delta.x > 0 && pos.x > this->box.r){
            pos.x = this->box.r;
        }
        if(setpos){
            cxViewSetPos(view, pos);
        }
    }else if(touch->type == cxTouchTypeUp){
        cxBool setpos = false;
        cxFloat time = touch->utime - touch->dtime;
        cxVec2f speed = cxVec2fv(touch->movement.x / time, touch->movement.y / time);
        cxVec2f new = cxViewPosition(view);
        if((this->type & cxScrollMoveTypeVertical) && (fabsf(speed.y) >= this->value)){
            new.y += speed.y * this->super.size.h / this->value;
            setpos = fabsf(touch->delta.y) > 15;
        }
        if(touch->current.y < touch->start.y && new.y < this->box.b){
            new.y = this->box.b;
        }
        if(touch->current.y > touch->start.y > 0 && new.y > this->box.t){
            new.y = this->box.t;
        }
        if((this->type & cxScrollMoveTypeHorizontal) && (fabsf(speed.x) >= this->value)){
            new.x += speed.x * this->super.size.w / this->value;
            setpos = fabsf(touch->delta.x) > 15;
        }
        if(touch->current.x < touch->start.x && new.x < this->box.l){
            new.x = this->box.l;
        }
        if(touch->current.x > touch->start.x && new.x > this->box.r){
            new.x = this->box.r;
        }
        if(setpos){
            cxScrollActionView(this, new);
        }
    }
    return false;
}

static void cxScrollOnTouch(cxAny pview,cxTouch *touch,cxBool *ret)
{
    cxScroll this = pview;
    cxView view = cxScrollContainer(this);
    CX_RETURN(view == NULL || touch->type != cxTouchTypeDown);
    this->box.l = (view->size.w - this->super.size.w)/2.0f;
    this->box.r = -this->box.l;
    this->box.t = (view->size.h - this->super.size.h)/2.0f;
    this->box.b = -this->box.t;
    cxViewStopAction(view, CX_SCROLL_MOVE_ACTION_ID);
}

CX_OBJECT_INIT(cxScroll, cxView)
{
    cxEngine engine = cxEngineInstance();
    CX_METHOD_OVERRIDE(this->super.Touch, cxScrollTouch);
    cxViewSetCropping(this, true);
    this->type = cxScrollMoveTypeVertical;
    cxObjectSetReadAttrFunc(this, cxScrollReadAttr);
    //swip cond value
    this->value = 950;
    CX_SLOT_CONNECT(engine->onTouch, this, onTouch, cxScrollOnTouch);
}
CX_OBJECT_FREE(cxScroll, cxView)
{
    CX_SLOT_RELEASE(this->onTouch);
}
CX_OBJECT_TERM(cxScroll, cxView)











