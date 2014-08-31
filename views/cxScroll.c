//
//  cxScroll.c
//  cxEngine
//
//  Created by xuhua on 11/5/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <actions/cxMove.h>
#include "cxScroll.h"

cxView cxScrollContainer(cxAny pview)
{
    cxScroll this = pview;
    cxListElement *ele = cxListFirst(this->cxView.subViews);
    CX_ASSERT(ele != NULL, "cxScroll not set conatiner,should first subviews");
    return ele->any;
}

cxBool cxScrollTouch(cxAny pview,cxTouch *touch)
{
    cxScroll this = pview;
    cxView view = cxScrollContainer(this);
    CX_RETURN(view == NULL,false);
    cxVec2f pos;
    if(!cxViewHitTest(pview, touch->current, &pos)){
        return false;
    }
    if(touch->type == cxTouchTypeDown){
        cxVec2f vscale = cxViewScale(view);
        cxVec2f tscale = cxViewScale(this);
        this->box.r = (view->size.w * vscale.x - this->cxView.size.w * tscale.x)/2.0f;
        this->box.l = -this->box.r;
        this->box.t = (view->size.h * vscale.y - this->cxView.size.h * tscale.y)/2.0f;
        this->box.b = -this->box.t;
        return true;
    }else if(touch->type == cxTouchTypeMove){
        cxVec2f vpos = cxViewPosition(view);
        cxVec2f delta = cxViewTouchDelta(this, touch);
        cxBool setpos = false;
        if(this->type & cxScrollMoveTypeVertical){
            vpos.y += delta.y;
            if(CX_TOUCH_IS_DOWN(touch) && vpos.y < this->box.b){
                vpos.y = this->box.b;
            }else if(CX_TOUCH_IS_UP(touch) && vpos.y > this->box.t){
                vpos.y = this->box.t;
            }
            setpos = true;
        }
        if(this->type & cxScrollMoveTypeHorizontal){
            vpos.x += delta.x;
            if(CX_TOUCH_IS_LEFT(touch) && vpos.x < this->box.l){
                vpos.x = this->box.l;
            }else if(CX_TOUCH_IS_RIGHT(touch) && vpos.x > this->box.r){
                vpos.x = this->box.r;
            }
            setpos = true;
        }
        if(setpos){
            cxViewSetPos(view, vpos);
        }
        return setpos;
    }
    return false;
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
    CX_PROPERTY_SETTER(cxScroll, layout);
}
CX_OBJECT_INIT(cxScroll, cxView)
{
    CX_METHOD_SET(this->cxView.Touch, cxScrollTouch);
    cxViewSetCropping(this, true);
    this->type = cxScrollMoveTypeVertical;
}
CX_OBJECT_FREE(cxScroll, cxView)
{
    
}
CX_OBJECT_TERM(cxScroll, cxView)











