//
//  sqTimer.c
//  square
//
//  Created by xuhua on 4/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "sqTimer.h"

CX_OBJECT_INIT(sqTimer, cxSprite)
{
    cxEngine engine = cxEngineInstance();
    cxFloat h = engine->winsize.w / 32;
    cxViewSetSize(this, cxSize2fv(engine->winsize.w, h));
}
CX_OBJECT_FREE(sqTimer, cxSprite)
{
    
}
CX_OBJECT_TERM(sqTimer, cxSprite)