//
//  cxTable.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTable.h"

cxBool cxTableXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxViewXMLReadAttr(xmlView, mView, reader);
    cxTable this = mView;
    cxChar *type = cxXMLAttr("cxTable.type");
    //set space
    cxTableSetSpace(mView, cxXMLReadVec2fAttr(reader, "cxTable.space", this->space));
    //
    this->grid.x = cxXMLReadIntAttr(reader, "cxTable.col", 0);
    this->grid.y = cxXMLReadIntAttr(reader, "cxTable.row", 0);
    //set type
    if(cxConstCharsEqu(type, "vertical")){
        cxTableSetType(mView, cxTableArrayVertical);
    }else if(cxConstCharsEqu(type, "horizon")){
        cxTableSetType(mView, cxTableArrayHorizon);
    }else if(cxXMLReadIntsAttr(reader, "cxTable.type", &this->grid.x) == 2){
        cxTableSetType(mView,cxTableArrayGrid);
    }else{
        this->type = cxTableArrayNone;
    }
    xmlFree(type);
    //array hide
    cxTableArrayHide(mView, cxXMLReadBoolAttr(reader, "cxTable.arrayHide", this->arrayHide));
    return true;
}

void cxTableSetSpace(cxAny pview,cxVec2f space)
{
    cxTable this = pview;
    CX_RETURN(cxVec2fEqu(this->space, space));
    this->space = space;
    cxTableArraySubViews(pview);
}

void cxTableSetType(cxAny pview,cxTableArrayType type)
{
    cxTable this = pview;
    CX_RETURN(this->type == type);
    this->type = type;
    cxTableArraySubViews(pview);
}

void cxTableArrayHide(cxAny pview,cxBool arrayHide)
{
    cxTable this = pview;
    CX_RETURN(this->arrayHide == arrayHide);
    this->arrayHide = arrayHide;
    this->isArray = true;
}

void cxTableArraySubViews(cxAny pview)
{
    cxTable this = pview;
    this->isArray = true;
}

static void cxTableResize(cxEvent *event)
{
    cxTable this = event->sender;
    this->isArray = true;
}

static cxInt cxTableCount(cxTable this)
{
    cxInt count = 0;
    CX_LIST_FOREACH(this->super.subViews, ele){
        cxView view = ele->object;
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
    CX_RETURN(!this->isArray || this->type == cxTableArrayNone);
    this->isArray = false;
    cxInt count = cxTableCount(this);
    CX_RETURN(count == 0);
    cxFloat x = -this->super.size.w / 2.0f;
    cxFloat y = -this->super.size.h / 2.0f;
    cxFloat dx = 0;
    cxFloat dy = 0;
    if(this->grid.x > 0 || this->type == cxTableArrayHorizon){
        count = count > this->grid.x ? count : this->grid.x;
        cxFloat hnum = this->grid.x > 0 ? ceilf((cxFloat)count/(cxFloat)this->grid.x) : 1.0f;
        cxFloat wnum = this->grid.x > 0 ? (cxFloat)this->grid.x : (cxFloat)count;
        dx = this->super.size.w / wnum;
        dy = this->super.size.h / hnum;
    }
    if(this->grid.y > 0 || this->type == cxTableArrayVertical){
        count = count > this->grid.y ? count : this->grid.y;
        cxFloat hnum = this->grid.y > 0 ? ceilf((cxFloat)count/(cxFloat)this->grid.y) : 1.0f;
        cxFloat wnum = this->grid.y > 0 ? (cxFloat)this->grid.y : (cxFloat)count;
        dy = this->super.size.h / wnum;
        dx = this->super.size.w / hnum;
    }
    cxInt i = 0;
    CX_LIST_FOREACH(this->super.subViews, ele){
        cxView view = ele->object;
        if(!view->isVisible && !this->arrayHide){
            continue;
        }
        cxVec2f pos = view->position;
        cxInt col = 0;
        cxInt row = 0;
        if(this->type == cxTableArrayVertical){
            col = 0;
            row = i;
        }else if(this->type == cxTableArrayHorizon){
            col = i;
            row = 0;
        }else if(this->type == cxTableArrayGrid && this->grid.x > 0){
            col = i % this->grid.x;
            row = i / this->grid.x;
        }else if(this->type == cxTableArrayGrid && this->grid.y > 0){
            col = i / this->grid.y;
            row = i % this->grid.y;
        }else{
            continue;
        }
        pos.x = x + dx/2.0f + col * dx;
        pos.y = y + dy/2.0f + row * dy;
        cxViewSetPosition(view, pos);
        i++;
    }
}

CX_OBJECT_INIT(cxTable, cxView)
{
    this->arrayHide = true;
    CX_EVENT_QUICK(this->super.onResize, cxTableResize);
    CX_EVENT_QUICK(this->super.onChanged, cxTableResize);
    CX_EVENT_QUICK(this->super.onUpdate, cxTableUpdate);
    this->type = cxTableArrayNone;
    cxObjectSetXMLReadFunc(this, cxTableXMLReadAttr);
}
CX_OBJECT_FREE(cxTable, cxView)
{
    
}
CX_OBJECT_TERM(cxTable, cxView)





