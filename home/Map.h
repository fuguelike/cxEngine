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

CX_C_BEGIN

CX_OBJECT_DEF(Map, cxAtlas)
    cxSize2f unitSize;  //单元格子大小
    cxVec2i unitNum;//单元数量
    cxAny *items;
    struct{
        cxAny node;
        cxVec2i prev;
    }move;
CX_OBJECT_END(Map, cxAtlas)

cxBool MapSelectedNode(Map this,cxAny node,cxVec2f idx);

void MapClickedNode(Map this,cxAny node);

cxInt MapOffsetIdx(Map this,cxInt x,cxInt y);

cxVec2f MapPosToIdx(Map this,cxVec2f pos);

cxVec2f MapIdxToPos(Map this,cxVec2f idx);

cxHashKey MapIdxToKey(Map this,cxVec2i idx);

cxAny MapItem(Map this,cxVec2f idx);

void MapSetNode(Map this,cxVec2i idx,cxAny node);

cxBool MapRemoveNode(Map this,cxAny node);

cxBool MapIsValidIdx(Map this,cxVec2f idx);

CX_C_END

#endif
