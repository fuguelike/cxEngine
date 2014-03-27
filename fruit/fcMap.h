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
#include "fcDefine.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcMap, cxView)
    cxSize2f gridSize;
    cxAny sprites[DM_MAP_WIDTH][DM_MAP_HEIGHT];
    cxList fights;//可加入战斗的单位，包括被打的和打别人的
CX_OBJECT_END(fcMap)

//获取实际的像素
cxFloat fcMapScaleValue(fcMap this,cxFloat v);

cxBool fcMapFindPath(fcMap this,fcPath *path,cxVec2i start,cxVec2i stop);

cxVec2i fcMapToIdx(fcMap this,cxVec2f pos);

cxVec2f fcMapToPos(fcMap this,cxVec2i idx);

cxInt fcMapValue(fcMap this,cxVec2i idx);

cxAny fcMapSprite(fcMap this,cxVec2i idx);

cxBool fcMapCheckIdx(cxVec2i idx);

//添加战斗单位
void fcMapAppendFights(fcMap this,cxAny sprite);

void fcMapRemoveFights(fcMap this,cxAny sprite);

CX_C_END

#endif
