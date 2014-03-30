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
    cxList intruder;//闯关列表
    cxList defenser;//防御者
    cxList props;//道具列表
CX_OBJECT_END(fcMap)

//获取实际的像素
cxFloat fcMapScaleValue(fcMap this,cxFloat v);

cxBool fcMapFindPath(fcMap this,fcPath *path,cxVec2i start,cxVec2i stop);

cxVec2i fcMapToIdx(fcMap this,cxVec2f pos);

cxVec2f fcMapToPos(fcMap this,cxVec2i idx);

cxInt fcMapValue(fcMap this,cxVec2i idx);

cxAny fcMapSprite(fcMap this,cxVec2i idx);

cxBool fcMapCheckIdx(cxVec2i idx);

//添加闯关者
void fcMapAppendIntruder(fcMap this,cxAny sprite);

void fcMapRemoveIntruder(fcMap this,cxAny sprite);

//添加防御者
void fcMapAppendDefenser(fcMap this,cxAny sprite);

void fcMapRemoveDefenser(fcMap this,cxAny sprite);

//添加道具
void fcMapAppendProps(fcMap this,cxAny sprite);

void fcMapRemoveProps(fcMap this,cxAny sprite);

CX_C_END

#endif
