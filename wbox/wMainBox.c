//
//  wMainBox.c
//  wbox
//
//  Created by xuhua on 4/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <views/cxSprite.h>
#include "wMainBox.h"


CX_OBJECT_INIT(wMainBox, cxView)
{
    cxEngine engine = cxEngineInstance();
    cxFloat h = engine->winsize.h / 23;
    cxFloat w = h;
    this->unitSize = cxSize2fv(w, h);
    cxViewSetSize(this, cxSize2fv(w * W_BOX_COL, h * W_BOX_ROW));
    cxViewSetAutoResizeMask(this, cxViewAutoResizeBottom);
    
    for(int i=0;i < 10; i++){
        for (int j=0; j < 20; j++) {
            cxSprite s = cxSpriteCreateWithURL("texture.xml?blue.png");
            cxVec2f pos = wMainBoxToPos(this, cxVec2iv(i, j));
            cxViewSetPos(s, pos);
            cxViewSetSize(s, this->unitSize);
            cxViewAppend(this, s);
        }
    }
}
CX_OBJECT_FREE(wMainBox, cxView)
{
    
}
CX_OBJECT_TERM(wMainBox, cxView)

cxVec2i wMainBoxToIdx(cxAny box,cxVec2f pos)
{
    wMainBox this = box;
    cxSize2f size = cxViewSize(this);
    cxFloat x = pos.x + size.w / 2.0f;
    cxFloat y = pos.y + size.h / 2.0f;
    return cxVec2iv(x/this->unitSize.w, y/this->unitSize.h);
}

cxVec2f wMainBoxToPos(cxAny box,cxVec2i idx)
{
    wMainBox this = box;
    cxSize2f size = cxViewSize(this);
    cxFloat x = idx.x * this->unitSize.w - size.w / 2.0f;
    cxFloat y = idx.y * this->unitSize.h - size.h / 2.0f;
    return cxVec2fv(x + this->unitSize.w/2.0f, y + this->unitSize.h/2.0f);
}