//
//  wmMap.c
//  winmine
//
//  Created by xuhua on 14-5-20.
//  Copyright (c) 2014年 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "wmMap.h"
#include "wmMine.h"

static cxBool wmMapTouch(cxAny pview,cxTouch *touch)
{
    wmMap this = pview;
    cxVec2f pos = cxVec2fv(0, 0);
    if(!cxViewHitTest(pview, touch->current, &pos)){
        return false;
    }
    if(touch->type == cxTouchTypeDown){
        cxVec2i idx = wmMapToIndex(this, pos);
        if(!wmMapCheckIdx(this, idx)){
            return false;
        }
        wmMine item = wmMapItem(this, idx);
        CX_LOGGER("%d",item->value);
    }
    return false;
}

CX_OBJECT_TYPE(wmMap, cxView)
{
    
}
CX_OBJECT_INIT(wmMap, cxView)
{
    CX_METHOD_SET(this->super.Touch, wmMapTouch);
    //8 - 12
    wmMapInit(this, cxSize2iv(12, 12));
}
CX_OBJECT_FREE(wmMap, cxView)
{
    allocator->free(this->items);
}
CX_OBJECT_TERM(wmMap, cxView)

cxVec2i wmMapToIndex(cxAny map,cxVec2f pos)
{
    wmMap this = map;
    cxSize2f size = cxViewSize(this);
    cxVec2i idx = cxVec2iv(0, 0);
    idx.x = (pos.x + size.w / 2.0f - this->gridSize.w /2.0f) / this->gridSize.w;
    idx.y = (pos.y + size.h / 2.0f - this->gridSize.h /2.0f) / this->gridSize.h;
    return idx;
}

cxVec2f wmMapToPos(cxAny map,cxVec2i index)
{
    wmMap this = map;
    cxSize2f size = cxViewSize(this);
    cxVec2f pos = cxVec2fv(0, 0);
    pos.x = index.x * this->gridSize.w - size.w / 2.0f  + this->gridSize.w / 2.0f;
    pos.y = index.y * this->gridSize.h - size.h / 2.0f  + this->gridSize.h / 2.0f;
    return pos;
}

static void wmMapSetGrid(cxAny map,cxSize2i grid)
{
    wmMap this = map;
    this->grid = grid;
    this->items = allocator->calloc(this->grid.w * this->grid.h,sizeof(cxAny));
}

cxBool wmMapCheckIdx(cxAny map,cxVec2i idx)
{
    wmMap this = map;
    return idx.x >= 0 && idx.x < this->grid.w && idx.y >= 0 && idx.y < this->grid.h;
}

cxAny wmMapItem(cxAny map,cxVec2i idx)
{
    wmMap this = map;
    CX_ASSERT(wmMapCheckIdx(map, idx),"idx error");
    cxAny *p = this->items + idx.x * this->grid.w + idx.y;
    return *p;
}

void wmMapInit(cxAny map,cxSize2i grid)
{
    wmMap this = map;
    wmMapSetGrid(this, grid);
    cxEngine engine = cxEngineInstance();
    this->gridSize.w = (engine->winsize.w - 10) / this->grid.w;
    this->gridSize.h = this->gridSize.w;
    cxViewSetSize(this, cxSize2fv(this->gridSize.w * this->grid.w, this->gridSize.h * this->grid.h));
    for(cxInt x=0; x < this->grid.w; x++){
        for (cxInt y=0; y < this->grid.h; y++) {
            wmMine item = CX_CREATE(wmMine);
            item->value = x * this->grid.w + y;
            item->map = this;
            cxViewSetSize(item, this->gridSize);
            cxVec2f pos = wmMapToPos(this, cxVec2iv(x, y));
            cxViewSetPos(item, pos);
            cxViewAppend(this, item);
            cxAny *p = this->items + x * this->grid.w + y;
            *p = item;
        }
    }
}




