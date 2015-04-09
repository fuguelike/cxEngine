//
//  cxTable.c
//  cxEngine
//
//  Created by xuhua on 10/22/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTable.h"

static cxInt cxTableCount(cxTable this,cxSize2f *psiz)
{
    cxSize2f maxsize = cxSize2fv(0, 0);
    cxSize2f size = cxSize2fv(0, 0);
    cxInt cols = this->Grid.x > 0 ? this->Grid.x : INT16_MAX;
    cxInt rows = this->Grid.y > 0 ? this->Grid.y : INT16_MAX;
    cxInt max = cols * rows;
    cxInt count = 0;
    cxInt cc = cols;
    cxInt rc = 0;
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        if(!view->IsVisible){
            continue;
        }
        cxViewSetAutoMask(view, cxViewAutoResizeNone);//cxTable subviews will disable autoresize
        cxSize2f siz = cxViewGetSize(view);
        cxBox4f box = cxViewGetAutoBox(view);
        cxFloat w = siz.w + box.l + box.r;
        cxFloat h = siz.h + box.t + box.b;
        if(h > size.h){
            size.h = h;
        }
        if(rows > 0){
            size.w += w;
            if(size.w > maxsize.w)maxsize.w = size.w;
            cc --;
        }
        if(cc == 0){
            size.w = 0;
            rc ++;
            maxsize.h += size.h;
            size.h = 0;
            cc = cols;
        }
        if(rc > rows){
            break;
        }
        if(count > max){
            break;
        }
        count ++;
    }
    if(size.w > maxsize.w){
        maxsize.w = size.w;
    }
    if(size.h > maxsize.h){
        maxsize.h = size.h;
    }
    if(count > 0 && psiz != NULL){
        *psiz = maxsize;
    }
    return count;
}

void cxTableArraySubviews(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxTable);
    cxSize2f size = cxViewGetSize(this);
    cxInt count = cxTableCount(this, &size);
    CX_RETURN(count == 0);
    cxViewSetSize(this, size);
    cxVec2f pos = cxVec2fv(0, 0);
    cxInt cols = this->Grid.x > 0 ? this->Grid.x : count;
    cxInt rows = this->Grid.y > 0 ? this->Grid.y : count;
    cxInt i = 0;
    cxInt cc = cols;
    cxInt rc = 0;
    cxFloat maxh = 0;
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        if(!view->IsVisible){
            continue;
        }
        cxViewSetAutoMask(view, cxViewAutoResizeNone);
        cxSize2f siz = cxViewGetSize(view);
        cxBox4f box = cxViewGetAutoBox(view);
        pos.x += box.l;
        pos.y += box.b;
        cxVec2f vpos = cxVec2fv(pos.x - size.w/2.0 + siz.w/2.0f, pos.y - size.h/2.0 + siz.h/2.0f);
        cxViewSetPosition(view, vpos);
        pos.x += box.r;
        pos.y += box.t;
        if(siz.h > maxh){
            maxh = siz.h;
        }
        if(cols > 0){
            pos.x += siz.w;
            cc --;
        }
        if(cc == 0){
            pos.x = 0;
            cc = cols;
            pos.y += maxh;
            rc ++;
            maxh = 0;
        }
        if(rc > rows){
            break;
        }
        if(i > count){
            break;
        }
        i++;
    }
}

CX_SETTER_DEF(cxTable, grid)
{
    cxTableSetGrid(this, cxJsonToVec2i(value, this->Grid));
}
CX_METHOD_DEF(cxTable, OnDirty, void)
{
    cxViewDirty dirty = cxViewGetDirty(this);
    if((dirty & cxViewDirtySize) || (dirty & cxViewDirtyForce)){
        cxTableArraySubviews(this);
    }
    CX_SUPER(cxView, this, OnDirty, CX_M(void));
}
CX_TYPE(cxTable, cxView)
{
    CX_SETTER(cxTable, grid);
    
    CX_METHOD(cxTable, OnDirty);
}
CX_INIT(cxTable, cxView)
{
    
}
CX_FREE(cxTable, cxView)
{
    
}
CX_TERM(cxTable, cxView)


cxTable cxTableCreate(cxVec2i grid)
{
    cxTable this = CX_CREATE(cxTable);
    this->Grid = grid;
    return this;
}







