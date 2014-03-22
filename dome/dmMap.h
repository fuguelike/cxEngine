//
//  dmMap.h
//  cxEngine
//
//  Created by xuhua on 3/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef dome_dmMap_h
#define dome_dmMap_h

#include <core/cxView.h>

CX_C_BEGIN

#define DM_MAP_WIDTH    10
#define DM_MAP_HEIGHT   10

typedef struct {
    cxVec2i points[DM_MAP_WIDTH*DM_MAP_HEIGHT];
    cxInt number;
}dmPath;

CX_OBJECT_DEF(dmMap, cxView)
    cxSize2f gridSize;
    cxInt values[DM_MAP_WIDTH][DM_MAP_HEIGHT];
    dmPath path;
CX_OBJECT_END(dmMap)

cxBool dmMapFindPath(dmMap this,cxVec2i start,cxVec2i stop);

cxVec2i dmMapToIdx(dmMap this,cxVec2f pos);

cxVec2f dmMapToPos(dmMap this,cxVec2i idx);

cxInt dmMapValue(dmMap this,cxVec2i idx);

void dmMapSetValue(dmMap this,cxVec2i idx,cxInt value);

cxBool dmMapCheckIdx(cxVec2i idx);

CX_C_END

#endif
