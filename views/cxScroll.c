//
//  cxScroll.c
//  cxEngine
//
//  Created by xuhua on 11/5/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <actions/cxMove.h>
#include "cxScroll.h"

#define CX_SCROLL_MOVE_ACTION_ID 100000

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
    CX_RETURN(kmAlmostEqual(distance, 0));
    cxMove move = cxMoveCreate((distance/this->value)*1.5f, new);
    cxActionSetId(move, CX_SCROLL_MOVE_ACTION_ID);
    cxActionSetCurve(move, cxCurveSineOut);
    cxViewAppendAction(view, move);
}

static void cxScrollTouchMove(cxScroll this,cxView view,cxTouch *touch)
{
    cxVec2f pos = cxViewPosition(view);
    if(this->type & cxScrollMoveTypeVertical){
        pos.y += touch->delta.y;
    }
    if(touch->delta.y < 0 && pos.y < (this->box.b - this->max)){
        pos.y = this->box.b - this->max;
    }else if(touch->delta.y > 0 && pos.y > (this->box.t + this->max)){
        pos.y = this->box.t + this->max;
    }
    if(this->type & cxScrollMoveTypeHorizontal){
        pos.x += touch->delta.x;
    }
    if(touch->delta.x < 0 && pos.x < (this->box.l - this->max)){
        pos.x = this->box.l - this->max;
    }else if(touch->delta.x > 0 && pos.x > (this->box.r + this->max)){
        pos.x = this->box.r + this->max;
    }
    cxViewSetPos(view, pos);
}

cxBool cxScrollTouch(cxAny pview,cxTouch *touch)
{
    cxScroll this = pview;
    cxView view = cxScrollContainer(this);
    if(!cxViewHitTest(pview, touch->current, NULL)){
        return false;
    }
    if(touch->type == cxTouchTypeUp){
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
        }else if(touch->current.y > touch->start.y > 0 && new.y > this->box.t){
            new.y = this->box.t;
        }
        if((this->type & cxScrollMoveTypeHorizontal) && (fabsf(speed.x) >= this->value)){
            new.x += speed.x * this->super.size.w / this->value;
            setpos = fabsf(touch->delta.x) > 15;
        }
        if(touch->current.x < touch->start.x && new.x < this->box.l){
            new.x = this->box.l;
        }else if(touch->current.x > touch->start.x && new.x > this->box.r){
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
    CX_RETURN(view == NULL);
    if(touch->type == cxTouchTypeDown){
        cxViewStopAction(view, CX_SCROLL_MOVE_ACTION_ID);
        this->box.r = (view->size.w - this->super.size.w)/2.0f;
        this->box.l = -this->box.r;
        this->box.t = (view->size.h - this->super.size.h)/2.0f;
        this->box.b = -this->box.t;
        if(this->type & cxScrollMoveTypeVertical){
            this->max = this->super.size.w / 8.0f;
        }else if(this->type & cxScrollMoveTypeHorizontal){
            this->max = this->super.size.h / 8.0f;
        }else{
            this->max = (this->super.size.w + this->super.size.h) / 16.0f;
        }
        this->selected = cxViewHitTest(pview, touch->current, NULL);
    }else if(touch->type == cxTouchTypeMove && this->selected){
        cxScrollTouchMove(this, view, touch);
    }else if(touch->type == cxTouchTypeUp){
        cxViewStopAction(view, CX_SCROLL_MOVE_ACTION_ID);
        cxBool setpos = false;
        cxVec2f new = cxViewPosition(view);
        if(new.y < this->box.b){
            new.y = this->box.b;
            setpos = true;
        }else if(new.y > this->box.t){
            new.y = this->box.t;
            setpos = true;
        }
        if(new.x < this->box.l){
            new.x = this->box.l;
            setpos = true;
        }else if(new.x > this->box.r){
            new.x = this->box.r;
            setpos = true;
        }
        if(setpos){
            cxScrollActionView(this, new);
        }
        this->selected = false;
    }
}

void __cxScrollInitObject(cxAny object,cxAny json,cxAny hash)
{
    cxScroll this = object;
    cxConstChars type = cxJsonConstChars(json, "layout");
    this->type = type != NULL ? cxScrollMoveTypeNone : this->type;
    if(cxConstCharsHas(type, "horizontal")){
        this->type |= cxScrollMoveTypeHorizontal;
    }
    if(cxConstCharsHas(type, "vertical")){
        this->type |= cxScrollMoveTypeVertical;
    }
    CX_OBJECT_INIT_SUPER(cxView);
}

CX_OBJECT_INIT(cxScroll, cxView)
{
    cxEngine engine = cxEngineInstance();
    CX_METHOD_OVERRIDE(this->super.Touch, cxScrollTouch);
    cxViewSetCropping(this, true);
    this->type = cxScrollMoveTypeVertical;
    //swip cond value
    this->value = 950;
    CX_SLOT_CONNECT(engine->onTouch, this, onTouch, cxScrollOnTouch);
    CX_OBJECT_INIT_OVERRIDE(cxScroll, this);
}
CX_OBJECT_FREE(cxScroll, cxView)
{
    CX_SLOT_RELEASE(this->onTouch);
}
CX_OBJECT_TERM(cxScroll, cxView)











