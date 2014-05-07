//
//  cxTable.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTable.h"

void cxTableSetSpace(cxAny pview,cxVec2f space)
{
    cxTable this = pview;
    CX_RETURN(cxVec2fEqu(this->space, space));
    this->space = space;
    cxTableArraySubviews(pview);
}

void cxTableArrayHide(cxAny pview,cxBool arrayHide)
{
    cxTable this = pview;
    CX_RETURN(this->arrayHide == arrayHide);
    this->arrayHide = arrayHide;
    cxTableArraySubviews(pview);
}

void cxTableArraySubviews(cxAny pview)
{
    cxTable this = pview;
    this->isArray = true;
}

static void cxTableResize(cxEvent *event)
{
    cxTableArraySubviews(event->sender);
}

static cxInt cxTableCount(cxTable this)
{
    cxInt count = 0;
    CX_LIST_FOREACH(this->super.subViews, ele){
        cxView view = ele->any;
        if(!view->isVisible && !this->arrayHide){
            continue;
        }
        count ++;
    }
    return count;
}

static void cxTableUpdate(cxEvent *event)
{
    cxTable this = event->sender;
    CX_RETURN(!this->isArray || (this->grid.x == 0 && this->grid.y == 0));
    this->isArray = false;
    cxInt count = cxTableCount(this);
    CX_RETURN(count == 0);
    cxFloat x = -this->super.size.w / 2.0f;
    cxFloat y = -this->super.size.h / 2.0f;
    cxFloat dx = 0;
    cxFloat dy = 0;
    if(this->grid.x > 0){
        count = count > this->grid.x ? count : this->grid.x;
        cxFloat hnum = ceilf((cxFloat)count/(cxFloat)this->grid.x);
        cxFloat wnum = this->grid.x;
        dx = this->super.size.w / wnum;
        dy = this->super.size.h / hnum;
    }
    if(this->grid.y > 0){
        count = count > this->grid.y ? count : this->grid.y;
        cxFloat hnum = ceilf((cxFloat)count/(cxFloat)this->grid.y);
        cxFloat wnum = this->grid.y;
        dy = this->super.size.h / wnum;
        dx = this->super.size.w / hnum;
    }
    cxInt i = 0;
    CX_LIST_FOREACH(this->super.subViews, ele){
        cxView view = ele->any;
        if(!view->isVisible && !this->arrayHide){
            continue;
        }
        cxVec2f pos = view->position;
        cxInt col = 0;
        cxInt row = 0;
        if(this->grid.x > 0){
            col = i % this->grid.x;
            row = i / this->grid.x;
        }else if(this->grid.y > 0){
            col = i / this->grid.y;
            row = i % this->grid.y;
        }
        pos.x = x + dx/2.0f + col * dx;
        pos.y = y + dy/2.0f + row * dy;
        cxViewSetPos(view, pos);
        i++;
    }
}

void cxTableInitView(cxAny pview,cxJson json)
{
    cxTable this = pview;
    this->grid.x = cxJsonInt(json, "grid.x", this->grid.x);
    this->grid.y = cxJsonInt(json, "grid.y", this->grid.y);
    CX_VIEW_SUPER_INIT(cxView);
}

CX_OBJECT_INIT(cxTable, cxView)
{
    this->arrayHide = true;
    CX_METHOD_OVERRIDE(this->super.InitView, cxTableInitView);
    CX_EVENT_QUICK(this->super.onResize, cxTableResize);
    CX_EVENT_QUICK(this->super.onUpdate, cxTableUpdate);
}
CX_OBJECT_FREE(cxTable, cxView)
{
    
}
CX_OBJECT_TERM(cxTable, cxView)





