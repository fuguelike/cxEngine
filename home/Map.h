//
//  Map.h
//  Home
//  地图处理
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Map_h
#define Home_Map_h

#include <engine/cxSpatial.h>
#include <views/cxAtlas.h>
#include <algorithm/cxAStar.h>
#include "Define.h"

CX_C_BEGIN

CX_OBJECT_DEF(Map, cxAtlas)
    cxView bullet;             //弹药效果层
    //mode
    MapMode mode;
    //
    cxBool isSort;
    cxHash nodes;
    //
    cxAny node;         //当前选中的node
    //所有单位空间索引
    cxSpatial items;
    //fight var
    cxBool isSelectUnit;
    //path search algorithm
    cxAStar astar;
    //路径索引
    cxHash paths;
    //Test
    cxInt tag;
CX_OBJECT_END(Map, cxAtlas)

typedef struct {
    cxAny map;
    cxAny snode;
    cxAny dnode;
    cxFloat dis;//两点间距离
    cxAny block;//第一个阻挡物
}MapSearchInfo;

//检测是否能加入地图
cxBool MapCanSetNode(cxAny node);

//加入单位
void MapAppendNode(cxAny node);

//移除单位
void MapRemoveNode(cxAny node);

//搜索离curr最近的单位
cxAny MapNearestQuery(cxAny curr,cxRange2f range,NodeType type,NodeSubType subType);

//搜索src dst之间的单位
cxAny MapSegmentQuery(cxAny src,cxAny dst,NodeType type, NodeSubType subType);

cxAnyArray MapVisiedPoints(cxAny pmap);

cxAnyArray MapSearchPoints(cxAny pmap);

//删除缓存路径
void MapCacheDelPath(cxAny pmap,cxVec2i a,cxVec2i b);

//保存路径缓存点
void MapCacheSetPath(cxAny pmap,cxVec2i a,cxVec2i b,cxAnyArray path);

//查询 a b两点是否有缓存路径
cxBool MapCachePath(cxAny pmap,cxVec2i a,cxVec2i b);

//寻路失败时返回离目标最近的坐标
//如果返回阻挡标识搜索失败
cxBool MapSearchPath(cxAny snode,cxAny dnode,cxAny *block);

//init map
cxBool MapInit(cxAny pmap,cxJson data);

void MapNodeOnNewIdx(cxAny pmap,cxAny pnode);

//按Y的大小重新排序
void MapSortNode(cxAny pmap);

void MapDelNode(cxAny pmap,cxInt x,cxInt y);

void MapSetNode(cxAny pmap,cxInt x,cxInt y,cxAny node);

cxAny MapNode(cxAny pview,cxInt x,cxInt y);

cxVec2f MapPosToIdx(cxAny pmap,cxVec2f pos);

cxVec2f MapIdxToPos(cxAny pmap,cxVec2f idx);

cxAny MapItem(cxAny pmap,cxVec2f idx);

//分离Node
void MapDetachNode(cxAny node);

void MapFillNode(cxAny pmap,cxVec2i idx,cxAny node);

cxBool MapIsValidIdx(cxAny pmap,cxVec2f idx);

CX_C_END

#endif
