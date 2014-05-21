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
    cxSize2i grid;
    cxSize2f gridSize;
    cxAny *items;
CX_OBJECT_END(wmMap)

cxBool wmMapCheckIdx(cxAny map,cxVec2i idx);

cxAny wmMapItem(cxAny map,cxVec2i idx);

void wmMapInit(cxAny map,cxSize2i grid);

cxVec2i wmMapToIndex(cxAny map,cxVec2f pos);

cxVec2f wmMapToPos(cxAny map,cxVec2i index);

CX_C_END

#endif
