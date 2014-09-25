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
    //静态防御方单位空间索引
    cxSpatial defences;
    //动态攻击方单位空间索引
    cxSpatial attacks;
    //阻挡单位空间索引
    cxSpatial blocks;
    //资源类空间索引
    cxSpatial resources;
    //装饰类空间索引
    cxSpatial decorations;
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
    cxFloat vdis;//访问到得最小距离
    cxVec2f vidx;//最靠近的店
}MapSearchInfo;

//加入防御单位
void MapAppendDefence(cxAny node);

//移除防御单位
void MapRemoveDefence(cxAny node);

//搜索离curr最近的防御类单位
cxAny MapNearestDefences(cxAny curr,cxRange2f range,NodeSubType subType);

//搜索src dst之间的防御类单位
cxAny MapSegmentDefences(cxAny src,cxAny dst,NodeSubType subType);


//加入攻击单位
void MapAppendAttack(cxAny node);

//移除攻击单位
void MapRemoveAttack(cxAny node);

//搜索离curr最近的攻击类单位
cxAny MapNearestAttacks(cxAny curr,cxRange2f range,NodeSubType subType);

//搜索src dst之间的攻击类单位
cxAny MapSegmentAttacks(cxAny src,cxAny dst,NodeSubType subType);


//加入阻挡单位，如城墙
void MapAppendBlock(cxAny node);

//移除阻挡单位
void MapRemoveBlock(cxAny node);

//搜索离curr最近的阻挡类单位
cxAny MapNearestBlocks(cxAny curr,cxRange2f range,NodeSubType subType);

//搜索src dst之间的阻挡类单位
cxAny MapSegmentBlocks(cxAny src,cxAny dst,NodeSubType subType);

//加入资源类型
void MapAppendResource(cxAny node);

//移除资源类型
void MapRemoveResource(cxAny node);

//搜索离curr最近的资源类单位
cxAny MapNearestResources(cxAny curr,cxRange2f range,NodeSubType subType);

//搜索src dst之间的资源类单位
cxAny MapSegmentResources(cxAny src,cxAny dst,NodeSubType subType);

//加入装饰类型
void MapAppendDecoration(cxAny node);

//移除装饰类型
void MapRemoveDecoration(cxAny node);

//搜索离curr最近的装饰类单位
cxAny MapNearestDecorations(cxAny curr,cxRange2f range,NodeSubType subType);

//搜索src dst之间的装饰类单位
cxAny MapSegmentDecorations(cxAny src,cxAny dst,NodeSubType subType);


cxAnyArray MapVisiedPoints(cxAny pmap);

cxAnyArray MapSearchPoints(cxAny pmap);


//删除缓存路径
void MapCacheDelPath(cxAny pmap,cxVec2i a,cxVec2i b);

//保存路径缓存点
void MapCacheSetPath(cxAny pmap,cxVec2i a,cxVec2i b,cxAnyArray path);

//查询 a b两点是否有缓存路径
cxBool MapCachePath(cxAny pmap,cxVec2i a,cxVec2i b);

//寻路失败时返回离目标最近的坐标
cxBool MapSearchPath(cxAny snode,cxAny dnode,cxVec2f *npos);

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

void MapFillNode(cxAny pmap,cxVec2i idx,cxAny node);

cxBool MapIsValidIdx(cxAny pmap,cxVec2f idx);

CX_C_END

#endif
