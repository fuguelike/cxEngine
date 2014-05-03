//
//  sqDrop.c
//  square
//
//  Created by xuhua on 4/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "sqDrop.h"
#include "sqSprite.h"
#include "sqMain.h"

CX_OBJECT_INIT(sqDrop, cxView)
{
    cxEngine engine = cxEngineInstance();
    cxViewSetSize(this, cxSize2fv(engine->winsize.w, engine->winsize.w));
}
CX_OBJECT_FREE(sqDrop, cxView)
{
    
}
CX_OBJECT_TERM(sqDrop, cxView)

cxAny sqDropAppendItem(sqDrop this,cxVec2i idx)
{
    sqSprite sprite = CX_CREATE(sqSprite);
    sqMain main = this->main;
    cxSize2f size = cxViewSize(this);
    cxFloat x = idx.x * main->unitSize.w - size.w/2.0 + main->unitSize.w/2.0f;
    cxFloat y = idx.y * main->unitSize.h - size.h/2.0 + main->unitSize.h/2.0f;
    cxSpriteSetTextureURL(sprite, "texture.json?red.png", false);
    cxViewSetSize(sprite, main->unitSize);
    cxViewSetPos(sprite, cxVec2fv(x, y));
    cxViewAppend(this, sprite);
    return sprite;
}