//
//  cxScroll.c
//  cxEngine
//
//  Created by xuhua on 11/5/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxScroll.h"

void cxScrollUpdateSubviewsPos(cxAny pview,cxVec2f delta)
{
    cxView this = pview;
    CX_LIST_FOREACH(this->subViews, ele){
        cxView tmp = ele->object;
        cxVec2f pos = tmp->position;
        kmVec2Add(&pos, &pos, &delta);
        cxViewSetPosition(tmp, pos);
    }
}

cxBool cxScrollTouch(cxAny pview,cxTouch *touch)
{
    cxScroll this = pview;
    if(touch->type == cxTouchTypeDown && cxViewHitTest(pview, touch)){
        this->enable = true;
        return true;
    }
    if(touch->type == cxTouchTypeMove && this->enable){
        cxScrollUpdateSubviewsPos(pview, touch->delta);
        return true;
    }
    if(touch->type == cxTouchTypeUp){
        this->enable = false;
        return true;
    }
    return false;
}

CX_OBJECT_INIT(cxScroll, cxView)
{
    cxViewSetCropping(this, true);
    CX_METHOD_SET(this->super.Touch, cxScrollTouch);
}
CX_OBJECT_FREE(cxScroll, cxView)
{
    
}
CX_OBJECT_TERM(cxScroll, cxView)











