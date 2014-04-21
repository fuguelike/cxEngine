//
//  wMainBox.h
//  cxEngine
//
//  Created by xuhua on 4/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef wbox_wMainBox_h
#define wbox_wMainBox_h

#include <core/cxView.h>
#include "wBoxDefine.h"

CX_C_BEGIN

CX_OBJECT_DEF(wBoxMain, cxView)
    cxSize2f unitSize;
    cxAny items[W_BOX_COL][W_BOX_ROW];
    wUnit unit;
CX_OBJECT_END(wBoxMain)

cxVec2i wBoxMainToIdx(cxAny box,cxVec2f pos);

cxVec2f wBoxMainToPos(cxAny box,cxVec2i idx);

CX_C_END

#endif
