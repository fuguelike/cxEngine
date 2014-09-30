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
    CX_ASSERT_THIS(pview, cxTable);
    CX_RETURN(cxVec2fEqu(this->space, space));
    this->space = space;
    cxTableArraySubviews(pview);
}

void cxTableArrayHide(cxAny pview,cxBool arrayHide)
{
    CX_ASSERT_THIS(pview, cxTable);
    CX_RETURN(this->arrayHide == arrayHide);
    this->arrayHide = arrayHide;
    cxTableArraySubviews(pview);
}

void cxTableArraySubviews(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxTable);
    this->isArray = true;
}

static void cxTableResize(cxAny sender)
{
    cxTableArraySubviews(sender);
}

static cxInt cxTableCount(cxTable this)
{
    cxInt count = 0;
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        if(!view->IsVisible && !this->arrayHide){
            continue;
        }
        count ++;
    }
    return count;
}

static void cxTableUpdate(cxAny sender)
{
    CX_ASSERT_THIS(sender, cxTable);
    CX_RETURN(!this->isArray || (this->grid.x == 0 && this->grid.y == 0));
    this->isArray = false;
    cxInt count = cxTableCount(this);
    CX_RETURN(count == 0);
    cxSize2f size = cxViewGetSize(this);
    cxFloat x = -size.w / 2.0f;
    cxFloat y = -size.h / 2.0f;
    cxFloat dx = 0;
    cxFloat dy = 0;
    if(this->grid.x > 0){
        count = count > this->grid.x ? count : this->grid.x;
        cxFloat hnum = ceilf((cxFloat)count/(cxFloat)this->grid.x);
        cxFloat wnum = this->grid.x;
        dx = size.w / wnum;
        dy = size.h / hnum;
    }
    if(this->grid.y > 0){
        count = count > this->grid.y ? count : this->grid.y;
        cxFloat hnum = ceilf((cxFloat)count/(cxFloat)this->grid.y);
        cxFloat wnum = this->grid.y;
        dy = size.h / wnum;
        dx = size.w / hnum;
    }
    cxInt i = 0;
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        if(!view->IsVisible && !this->arrayHide){
            continue;
        }
        cxVec2f pos = cxViewGetPosition(this);
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
        cxViewSetPosition(view, pos);
        i++;
    }
}

CX_SETTER_DEF(cxTable, grid)
{
    this->grid = cxJsonToVec2i(value, this->grid);
    cxTableArraySubviews(this);
}

CX_OBJECT_TYPE(cxTable, cxView)
{
    CX_PROPERTY_SETTER(cxTable, grid);
}
CX_OBJECT_INIT(cxTable, cxView)
{
    this->arrayHide = true;
    CX_ADD(cxView, this, onResize, cxTableResize);
    CX_ADD(cxView, this, onUpdate, cxTableUpdate);
}
CX_OBJECT_FREE(cxTable, cxView)
{
    
}
CX_OBJECT_TERM(cxTable, cxView)


cxTable cxTableCreate(cxVec2i grid)
{
    cxTable this = CX_CREATE(cxTable);
    this->grid = grid;
    return this;
}







