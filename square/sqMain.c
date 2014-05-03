//
//  sqMain.c
//  square
//
//  Created by xuhua on 4/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "sqMain.h"

CX_OBJECT_INIT(sqMain, cxSprite)
{
    cxEngine engine = cxEngineInstance();
    cxFloat w= engine->winsize.w / 6;
    this->unitSize = cxSize2fv(w,w);
    
    this->menu = CX_CREATE(sqMenu);
    cxViewSetAutoResizeMask(this->menu, cxViewAutoResizeTop);
    cxViewAppend(this, this->menu);
    
    this->timer = CX_CREATE(sqTimer);
    cxViewSetAutoResizeBox(this->timer, cxBox4fv(0, 0, this->menu->super.size.h, 0));
    cxViewSetAutoResizeMask(this->timer, cxViewAutoResizeTop);
    cxViewAppend(this, this->timer);
    
    this->drop = CX_CREATE(sqDrop);
    cxViewSetAutoResizeMask(this->drop, cxViewAutoResizeBottom);
    cxViewAppend(this, this->drop);
    this->drop->main = this;
    
    this->current = CX_CREATE(sqSprite);
    cxSpriteSetTextureURL(this->current, "texture.json?blue.png", false);
    cxFloat y = this->menu->super.size.h + this->timer->super.super.size.h - this->unitSize.h/2;
    y+= (engine->winsize.h - engine->winsize.w - (this->menu->super.size.h + this->timer->super.super.size.h)) / 2;
    cxViewSetPos(this->current, cxVec2fv(0, y));
    cxViewSetAutoResizeBox(this->current, cxBox4fv(0, 0, y, 0));
    cxViewSetAutoResizeMask(this->current, cxViewAutoResizeTop);
    cxViewSetSize(this->current, this->unitSize);
    cxViewAppend(this, this->current);
    
    
    sqDropAppendItem(this->drop, cxVec2iv(5, 5));
    
}
CX_OBJECT_FREE(sqMain, cxSprite)
{
    
}
CX_OBJECT_TERM(sqMain, cxSprite)