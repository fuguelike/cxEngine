//
//  wmMap.h
//  winmine
//
//  Created by xuhua on 14-5-20.
//  Copyright (c) 2014年 xuhua. All rights reserved.
//

#ifndef winmine_wmMap_h
#define winmine_wmMap_h

#include <engine/cxView.h>
#include "wmDefine.h"

CX_C_BEGIN

CX_OBJECT_DEF(wmMap, cxView)
    cxUInt items[WM_MAP_COL][WM_MAP_ROW];
CX_OBJECT_END(wmMap)

CX_C_END

#endif
