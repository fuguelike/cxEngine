//
//  cxTable.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTable.h"

void cxTableXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
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
    if(this->grid.x > 0){
        cxTableSetType(mView, cxTableArrayGrid);
    }else if(this->grid.y > 0){
        cxTableSetType(mView, cxTableArrayGrid);
    }else if(type != NULL && strcasecmp(type, "vertical") == 0){
        cxTableSetType(mView, cxTableArrayVertical);
    }else if(type != NULL && strcasecmp(type, "horizon") == 0){
        cxTableSetType(mView, cxTableArrayHorizon);
    }else{
        cxTableSetType(mView, cxTableArrayNone);
    }
    xmlFree(type);
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

void cxTableArraySubViews(cxAny pview)
{
    cxTable this = pview;
    this->isArray = true;
}

static void cxTableResize(cxAny pview,cxAny args)
{
    cxTable this = pview;
    this->isArray = true;
}

static void cxTableUpdate(cxAny pview,cxAny args)
{
    cxTable this = pview;
    CX_RETURN(!this->isArray || this->type == cxTableArrayNone);
    this->isArray = false;
    cxInt count = cxListLength(this->super.subViews);
    CX_RETURN(count == 0);
    cxFloat x = -this->super.size.w / 2.0f;
    cxFloat y = -this->super.size.h / 2.0f;
    cxFloat dx = 0;
    cxFloat dy = 0;
    if(this->grid.x > 0){
        count = count > this->grid.x ? count : this->grid.x;
        dx = this->super.size.w / (cxFloat)this->grid.x;
        dy = this->super.size.h / ((cxFloat)count / (cxFloat)this->grid.x);
    }
    if(this->grid.y > 0){
        count = count > this->grid.y ? count : this->grid.y;
        dy = this->super.size.h / (cxFloat)this->grid.y;
        dx = this->super.size.w / ((cxFloat)count / (cxFloat)this->grid.y);
    }
    cxInt i = 0;
    CX_LIST_FOREACH(this->super.subViews, ele){
        cxView view = ele->object;
        cxVec2f pos = view->position;
        if(this->type == cxTableArrayVertical){
            y += view->size.h/2.0f;
            pos.y = y;
            y += this->space.y + view->size.h / 2.0f;
        }else if(this->type == cxTableArrayHorizon){
            x += view->size.w / 2.0f;
            pos.x = x;
            x += this->space.x + view->size.w / 2.0f;
        }else if(this->type == cxTableArrayGrid && this->grid.x > 0){
            cxInt col = i % this->grid.x;
            cxInt row = i / this->grid.x;
            pos.x = x + dx/2.0f + col * dx;
            pos.y = y + dy/2.0f + row * dy;
        }else if(this->type == cxTableArrayGrid && this->grid.y > 0){
            cxInt col = i / this->grid.y;
            cxInt row = i % this->grid.y;
            pos.x = x + dx/2.0f + col * dx;
            pos.y = y + dy/2.0f + row * dy;
        }else{
            continue;
        }
        cxViewSetPosition(view, pos);
        i++;
    }
}

CX_OBJECT_INIT(cxTable, cxView)
{
    CX_EVENT_APPEND(this->super.onResize, cxTableResize, NULL);
    CX_EVENT_APPEND(this->super.onChanged, cxTableResize, NULL);
    CX_EVENT_APPEND(this->super.onUpdate, cxTableUpdate, NULL);
    this->type = cxTableArrayNone;
    cxObjectSetXMLReadFunc(this, cxTableXMLReadAttr);
}
CX_OBJECT_FREE(cxTable, cxView)
{
    
}
CX_OBJECT_TERM(cxTable, cxView)





