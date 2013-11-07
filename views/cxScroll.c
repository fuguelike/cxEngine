//
//  cxScroll.c
//  cxEngine
//
//  Created by xuhua on 11/5/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <actions/cxMove.h>
#include "cxScroll.h"

#define MOVE_ACTION_ID 100000

cxBool cxScrollXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxViewXMLReadAttr(xmlView, mView, reader);
    cxScroll this = mView;
    cxChar *type = cxXMLAttr("cxScroll.type");
    if(cxConstCharsEqu(type, "horizontal")){
        this->type |= cxScrollMoveTypeHorizontal;
    }else if(cxConstCharsEqu(type, "vertical")){
        this->type |= cxScrollMoveTypeVertical;
    }else {
        this->type |= (cxScrollMoveTypeVertical|cxScrollMoveTypeHorizontal);
    }
    xmlFree(type);
    this->value = cxXMLReadFloatAttr(reader, "cxScroll.value", this->value);
    return true;
}

cxView cxScrollContainer(cxAny pview)
{
    cxScroll this = pview;
    cxListElement *ele = cxListFirst(this->super.subViews);
    CX_ASSERT(ele != NULL, "cxScroll not set conatiner,should first subviews");
    return ele->object;
}

cxBool cxScrollTouch(cxAny pview,cxTouch *touch)
{
    cxScroll this = pview;
    cxView view = cxScrollContainer(this);
    if(!cxViewHitTest(pview, touch->current)){
        return false;
    }
    this->box.l = (view->size.w-this->super.size.w)/2.0f;
    this->box.r = -this->box.l;
    this->box.t = (view->size.h-this->super.size.h)/2.0f;
    this->box.b = -this->box.t;
    if(touch->type == cxTouchTypeMove){
        cxViewStopAction(view, MOVE_ACTION_ID);
        cxVec2f pos = cxViewPosition(view);
        cxBool set = false;
        if(this->type & cxScrollMoveTypeVertical){
            pos.y += touch->delta.y;
            set = true;
        }
        if(touch->delta.y < 0 && pos.y < this->box.b){
            pos.y = this->box.b;
        }
        if(touch->delta.y > 0 && pos.y > this->box.t){
            pos.y = this->box.t;
        }
        if(this->type & cxScrollMoveTypeHorizontal){
            pos.x += touch->delta.x;
            set = true;
        }
        if(touch->delta.x < 0 && pos.x < this->box.l){
            pos.x = this->box.l;
        }
        if(touch->delta.x > 0 && pos.x > this->box.r){
            pos.x = this->box.r;
        }
        if(set){
            cxViewSetPosition(view, pos);
        }
    }else if(touch->type == cxTouchTypeUp){
        cxBool set = false;
        cxFloat time = touch->utime - touch->dtime;
        cxVec2f speed = cxVec2fv(touch->movement.x / time, touch->movement.y / time);
        cxVec2f new = cxViewPosition(view);
        if((this->type & cxScrollMoveTypeVertical) && (fabsf(speed.y) >= this->value)){
            new.y += speed.y * this->super.size.h / this->value;
            set = true;
        }
        if(touch->current.y < touch->start.y && new.y < this->box.b){
            new.y = this->box.b;
        }
        if(touch->current.y > touch->start.y > 0 && new.y > this->box.t){
            new.y = this->box.t;
        }
        if((this->type & cxScrollMoveTypeHorizontal) && (fabsf(speed.x) >= this->value)){
            new.x += speed.x * this->super.size.w / this->value;
            set = true;
        }
        if(touch->current.x < touch->start.x && new.x < this->box.l){
            new.x = this->box.l;
        }
        if(touch->current.x > touch->start.x && new.x > this->box.r){
            new.x = this->box.r;
        }
        if(set){
            cxMove move = cxMoveCreate(1.0f, new);
            cxActionSetId(move, MOVE_ACTION_ID);
            cxActionSetCurve(move, cxCurveCubeOut);
            cxViewAppendAction(view, move);
        }
    }
    return false;
}

CX_OBJECT_INIT(cxScroll, cxView)
{
    cxViewOverrideTouch(this, cxScrollTouch);
    cxViewSetCropping(this, true);
    this->type = cxScrollMoveTypeVertical;
    cxObjectSetXMLReadFunc(this, cxScrollXMLReadAttr);
    this->value = 1100;
}
CX_OBJECT_FREE(cxScroll, cxView)
{
    
}
CX_OBJECT_TERM(cxScroll, cxView)











