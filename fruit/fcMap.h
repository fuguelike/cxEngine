//
//  fcMap.h
//  cxEngine
//
//  Created by xuhua on 3/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcMap_h
#define fruit_fcMap_h

#include <core/cxView.h>

CX_C_BEGIN

#define DM_MAP_WIDTH    10
#define DM_MAP_HEIGHT   10

typedef struct {
    cxVec2i points[DM_MAP_WIDTH*DM_MAP_HEIGHT];
    cxInt number;
}fcPath;

CX_OBJECT_DEF(fcMap, cxView)
    cxSize2f gridSize;
    cxInt values[DM_MAP_WIDTH][DM_MAP_HEIGHT];
    fcPath path;
    cxList enemies;
CX_OBJECT_END(fcMap)

cxBool fcMapFindPath(fcMap this,cxVec2i start,cxVec2i stop);

cxVec2i fcMapToIdx(fcMap this,cxVec2f pos);

cxVec2f fcMapToPos(fcMap this,cxVec2i idx);

cxInt fcMapValue(fcMap this,cxVec2i idx);

void fcMapSetValue(fcMap this,cxVec2i idx,cxInt value);

cxBool fcMapCheckIdx(cxVec2i idx);

CX_C_END

#endif
