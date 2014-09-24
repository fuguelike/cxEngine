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
    MapMode mode;
    //
    cxBool isSort;
    //
    cxAny *items;       //格子node
    cxAny node;     //当前选中的node
    //防御方单位空间索引
    cxSpatial defences;
    //攻击方单位空间索引
    cxSpatial attacks;
    //阻挡单位空间索引
    cxSpatial blocks;
    //fight var
    cxBool isSelectUnit;
//path search algorithm
    cxAStar astar;
    //Test
    cxInt tag;
CX_OBJECT_END(Map, cxAtlas)

typedef struct {
    cxAny map;
    cxAny snode;
    cxAny dnode;
}MapSearchInfo;
cxAnyArray MapSearchPath(cxAny snode,cxAny dnode);

//init map
cxBool MapInit(cxAny pmap,cxJson data);

void MapNodeOnNewIdx(cxAny pmap,cxAny pnode);

//按Y的大小重新排序
void MapSortNode(cxAny pmap);

//加入攻击单位
void MapAppendAttack(cxAny node);

//加入防御单位
void MapAppendDefence(cxAny node);

//加入阻挡单位，如城墙
void MapAppendBlock(cxAny node);

void MapRemoveAttack(cxAny node);

void MapRemoveDefence(cxAny node);

void MapRemoveBlock(cxAny node);

cxInt MapOffSetIdx(cxInt x,cxInt y);

cxVec2f MapPosToIdx(cxAny pmap,cxVec2f pos);

cxVec2f MapIdxToPos(cxAny pmap,cxVec2f idx);

cxAny MapItem(cxAny pmap,cxVec2f idx);

void MapSetNode(cxAny pmap,cxVec2i idx,cxAny node);

cxBool MapIsValidIdx(cxAny pmap,cxVec2f idx);

CX_C_END

#endif
