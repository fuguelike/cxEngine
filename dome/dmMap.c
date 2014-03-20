//
//  dmMap.c
//  dome
//
//  Created by xuhua on 3/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "dmMap.h"

cxVec2i dmMapToIdx(dmMap this,cxVec2f pos)
{
    cxSize2f size = cxViewSize(this);
    cxFloat x = pos.x + size.w / 2.0f;
    cxFloat y = pos.y + size.h / 2.0f;
    return cxVec2iv(x/this->gridSize.w, y/this->gridSize.h);
}

cxVec2f dmMapToPos(dmMap this,cxVec2i idx)
{
    cxSize2f size = cxViewSize(this);
    cxFloat x = idx.x * this->gridSize.w - size.w / 2.0f;
    cxFloat y = idx.y * this->gridSize.h - size.h / 2.0f;
    return cxVec2fv(x + this->gridSize.w/2.0f, y + this->gridSize.h/2.0f);
}

static cxBool dmMapTouch(cxAny pview,cxTouch *touch)
{
    dmMap this = pview;
    cxVec2f pos;
    if(!cxViewHitTest(this, touch->current, &pos)){
        return false;
    }
    if(touch->type == cxTouchTypeDown){
        cxVec2i idx = dmMapToIdx(this, pos);
        cxVec2f pos = dmMapToPos(this, idx);
        cxView v = CX_CREATE(cxView);
        cxViewSetSize(v, this->gridSize);
        cxViewSetPos(v, pos);
        cxViewAppend(this, v);
        CX_LOGGER("x=%d y=%d",idx.x,idx.y);
    }
    return false;
}

CX_OBJECT_INIT(dmMap, cxView)
{
    CX_METHOD_OVERRIDE(this->super.Touch, dmMapTouch);
    cxSize2f size = cxEngineInstance()->winsize;
    this->gridSize.w = (size.w - 10) / DM_MAP_WIDTH;
    this->gridSize.h = this->gridSize.w;
    cxViewSetSize(this, cxSize2fv(size.w - 10,size.w - 10));
}
CX_OBJECT_FREE(dmMap, cxView)
{
    
}
CX_OBJECT_TERM(dmMap, cxView)

