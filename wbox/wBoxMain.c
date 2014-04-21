//
//  wMainBox.c
//  wbox
//
//  Created by xuhua on 4/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <views/cxSprite.h>
#include "wBoxMain.h"
#include "wBoxItem.h"

static cxBool wBoxMainTouch(cxAny pview,cxTouch *touch)
{
    if(touch->type == cxTouchTypeUp){
       
    }
    return false;
}


CX_OBJECT_INIT(wBoxMain, cxView)
{

    CX_METHOD_OVERRIDE(this->super.Touch, wBoxMainTouch);
    
    cxEngine engine = cxEngineInstance();
    cxFloat h = engine->winsize.h / 23;
    cxFloat w = h;
    this->unitSize = cxSize2fv(w, h);
    cxViewSetSize(this, cxSize2fv(w * W_BOX_COL, h * W_BOX_ROW));
    cxViewSetAutoResizeMask(this, cxViewAutoResizeBottom);
    
    wUnitInit(&this->unit, wUnitType_I);
    
    for(cxInt x=0; x < W_BOX_COL;x++){
        for(cxInt y=0; y < W_BOX_ROW; y++){
            wBoxItem item = wBoxItemCreate(this, cxVec2iv(x, y));
            cxViewSetVisible(item, false);
            cxViewAppend(this, item);
        }
    }
}
CX_OBJECT_FREE(wBoxMain, cxView)
{
    
}
CX_OBJECT_TERM(wBoxMain, cxView)

cxVec2i wBoxMainToIdx(cxAny box,cxVec2f pos)
{
    wBoxMain this = box;
    cxSize2f size = cxViewSize(this);
    cxFloat x = pos.x + size.w / 2.0f;
    cxFloat y = pos.y + size.h / 2.0f;
    return cxVec2iv(x/this->unitSize.w, y/this->unitSize.h);
}

cxVec2f wBoxMainToPos(cxAny box,cxVec2i idx)
{
    wBoxMain this = box;
    cxSize2f size = cxViewSize(this);
    cxFloat x = idx.x * this->unitSize.w - size.w / 2.0f;
    cxFloat y = idx.y * this->unitSize.h - size.h / 2.0f;
    return cxVec2fv(x + this->unitSize.w/2.0f, y + this->unitSize.h/2.0f);
}