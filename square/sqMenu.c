//
//  sqMenu.c
//  square
//
//  Created by xuhua on 4/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "sqMenu.h"

CX_OBJECT_INIT(sqMenu, cxView)
{
    cxEngine engine = cxEngineInstance();
    cxFloat h = engine->winsize.w / 8;
    cxViewSetSize(this, cxSize2fv(engine->winsize.w, h));
}
CX_OBJECT_FREE(sqMenu, cxView)
{
    
}
CX_OBJECT_TERM(sqMenu, cxView)