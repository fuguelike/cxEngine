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
    cxView aLayer;  //效果层
    cxView nLayer;  //nodes节点层
    //格子属性
    MapUnitAttr *attrs;
    //mode
    MapMode mode;
    //
    cxBool isSort;
    //静态单位存储
    cxHash nodes;
    //动态单位空间索引
    cxSpatial items;
    //fight var
    cxBool isSelectUnit;
    //path search algorithm
    cxAStar astar;
    //路径缓存索引
    cxHash paths;
    //Test
    cxInt tag;
    cxVec2f a;
    cxVec2f b;
CX_OBJECT_END(Map, cxAtlas)

typedef struct {
    cxAny map;
    cxVec2f mid;    //中心点
    cxFloat ab;    //两点间距离
    cxAny src;
    cxAny dst;
}MapSearchInfo;

//加入攻击
void MapAppendBullet(cxAny bullet);

//移除攻击物
void MapRemoveBullet(cxAny bullet);

//加入单位
void MapAppendNode(cxAny node);

//移除所有的nodes
void MapRemoveNodes(cxAny pmap);

//移除单位
void MapRemoveNode(cxAny node);

//搜索某点附近的，范围内的所有node,包括min,不包括max，没有返回NULL
cxArray MapNearestItems(cxAny pmap,cxVec2f point,cxRange2f range,NodeCombined type);

//搜索离curr最近的单位
cxAny MapNearestQuery(cxAny curr,NodeCombined type,cxRange2f range);

//搜索src dst之间的单位
cxAny MapSegmentQuery(cxAny src,cxAny dst,NodeCombined type);

cxAnyArray MapVisiedPoints(cxAny pmap);

//清空路径点
void MapClearPoints(cxAny pmap);

//手动添加路径点,pos
void MapAppendPoint(cxAny pmap,cxVec2f pos);

cxAnyArray MapSearchPoints(cxAny pmap);

//删除缓存路径
void MapCacheDelPath(cxAny pmap,cxVec2i a,cxVec2i b);

//保存路径缓存点
cxBool MapCacheSavePath(cxAny pmap,cxVec2i a,cxVec2i b);

//查询 a b两点是否有缓存路径
cxBool MapCachePath(cxAny pmap,cxVec2i a,cxVec2i b);

//寻路失败时返回离目标最近的坐标
//如果返回阻挡标识搜索失败
cxBool MapSearchPath(cxAny snode,cxAny dnode);

//init map
cxBool MapInit(cxAny pmap);

void MapSetMode(cxAny pmap,MapMode mode);

//按Y的大小重新排序
void MapSortNode(cxAny pmap);

void MapDelNode(cxAny pmap,cxInt x,cxInt y);

void MapSetNode(cxAny pmap,cxInt x,cxInt y,cxAny node);

cxVec2f MapPosToFloat(cxAny pmap,cxVec2f pos);

cxVec2i MapPosToIndex(cxAny pmap,cxVec2f pos);

/*
idx索引位置在边上,所以要使一个格子内容居中，
idx.x or idx.y 一般需要加上0.5f,两格子内容居中加上1.0f
1,0   1,1
--------
|      |
|      |
|      |
|      |
--------
0,0   1,0
*/
cxVec2f MapIndexToPos(cxAny pmap,cxVec2f idx);

cxAny MapItem(cxAny pmap,cxVec2i idx);

//设置 获取 添加 消去某个格子的属性
void MapSubAttr(cxAny pmap,cxVec2i idx,MapUnitAttr attr);
void MapAddAttr(cxAny pmap,cxVec2i idx,MapUnitAttr attr);
void MapSetAttr(cxAny pmap,cxVec2i idx,MapUnitAttr attr);
MapUnitAttr MapAttr(cxAny pmap,cxVec2i idx);

//分离Node
void MapDetachNode(cxAny node);

//将node放到idx位置
void MapFillNode(cxAny pmap,cxVec2i idx,cxAny node);

cxBool MapIsValidIdx(cxAny pmap,cxVec2i idx);

CX_C_END

#endif
