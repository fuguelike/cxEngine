//
//  Map.h
//  Home
//  地图处理
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Map_h
#define Home_Map_h

#include <views/cxAtlas.h>
#include "Define.h"

CX_C_BEGIN

CX_OBJECT_DEF(Map, cxAtlas)
    MapMode mode;
    cxSize2f unitSize;  //单元格子大小
    cxVec2i unitNum;    //单元数量
    cxAny *items;       //格子node
    cxAny node;     //当前选中的node
    //node list
    cxList nodes;
    //
    cxBox4f box;
CX_OBJECT_END(Map, cxAtlas)

//init map
cxBool MapInit(cxAny pmap,cxJson data);

void MapNodeOnNewIdx(cxAny pmap,cxAny pnode);

//按Y的大小重新排序
void MapSortNode(cxAny pmap);

void MapAppendNode(cxAny pmap,cxAny node);

cxInt MapOffsetIdx(cxAny pmap,cxInt x,cxInt y);

cxVec2f MapPosToIdx(cxAny pmap,cxVec2f pos);

cxVec2f MapIdxToPos(cxAny pmap,cxVec2f idx);

cxAny MapItem(cxAny pmap,cxVec2f idx);

void MapSetNode(cxAny pmap,cxVec2i idx,cxAny node);

cxBool MapRemoveNode(cxAny pmap,cxAny node);

cxBool MapIsValidIdx(cxAny pmap,cxVec2f idx);

CX_C_END

#endif
