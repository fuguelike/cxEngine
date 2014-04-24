//
//  rbMap.c
//  rainbow
//
//  Created by xuhua on 4/24/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "rbMap.h"

CX_OBJECT_INIT(rbMap, cxView)
{
    cxEngine engine = cxEngineInstance();
    this->unitSize.w = (engine->winsize.w - 18) / RB_COL;
    this->unitSize.h = this->unitSize.w;
    cxViewSetSize(this, cxSize2fv(this->unitSize.w * RB_COL, this->unitSize.h * RB_ROW));
}
CX_OBJECT_FREE(rbMap, cxView)
{
    
}
CX_OBJECT_TERM(rbMap, cxView)