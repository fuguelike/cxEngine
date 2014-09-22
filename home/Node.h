//
//  Node.h
//  cxCore
//
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Node_h
#define Home_Node_h

#include <engine/cxSpatial.h>
#include <views/cxSprite.h>
#include "Define.h"

CX_C_BEGIN

CX_OBJECT_DEF(Node, cxSprite)
    cxAny map;
    cxSize2f size;      //占用的格子数
    cxVec2f idx;        //左下角格子坐标
    cxAnyArray box;
    cxVec2f curr;       //当前所在位置
    cxBool canSelected; //可选择
    cxBool isSelected;  //是否悬选中
    cxBool isTouch;     //按下时是否选中
    cxBool isValidIdx;  //是否在有效的位置
    cxVec2f start;
    NodeSubType subType;//子类型
    NodeType type;      //主类型
    NodeState state;    //当前状态
    cxRange2f range;    //攻击范围
    cxInt attack;       //攻击力
    cxInt life;         //生命值
    cxInt level;        //等级
    cxTimer searchTimer;      //搜索用定时器
    CX_METHOD_DEF(void, Search,cxAny);
    cxTimer attackTimer;    //攻击用定时器
    CX_METHOD_DEF(void, Attack,cxAny);
CX_OBJECT_END(Node, cxSprite)

//设置生命 等级 攻击力
void NodeSetLife(cxAny pview,cxInt life);
void NodeAddLife(cxAny pview,cxInt life);
void NodeSetLevel(cxAny pview,cxInt level);
void NodeSetAttack(cxAny pview,cxInt attack);

typedef struct {
    cxAny node;     //最近的view
    cxVec2f p;      //与此点的
    cxFloat d;      //距离
    cxFloat m;      //max
    cxVec2f idx;    //node最近点
    NodeType type;      //搜索的类型
    NodeSubType subType;//搜索子类型
} NodeNearestInfo;

//节点死亡
cxBool NodeIsDie(cxAny pview);

//设置状态
void NodeSetState(cxAny pview,NodeState state);

NodeNearestInfo NodeNearest(cxAny ps,cxVec2f p,cxFloat max,NodeType type,NodeSubType subType);

//暂停搜索
void NodePauseSearch(cxAny pview);

//重新启动搜索
void NodeResumeSearch(cxAny pview);

//启动搜索定时器
void NodeSearchRun(cxAny pview,cxFloat freq);

//暂停攻击
void NodePauseAttack(cxAny pview);

//重新启动攻击
void NodeResumeAttack(cxAny pview);

//启动攻击定时器
void NodeAttackRun(cxAny pview,cxFloat freq);

//idx与node的最近格子索引
cxVec2f NodeNearestIdx(cxAny pview,cxVec2f idx, cxFloat *d);

cxRange2f NodeRange(cxAny pview);

cxFloat NodeAttack(cxAny pview);

cxFloat NodeLife(cxAny pview);

cxInt NodeLevel(cxAny pview);

cxVec2i NodeIndex(cxAny pview);

cxSize2i NodeSize(cxAny pview);

//从当前坐标转换为索引坐标
cxVec2f NodePosToIdx(cxAny pview,cxVec2f pos);

//update to curr
void NodeUpdateIdx(cxAny pview);

cxVec2f NodeCurrIdx(cxAny pview);
//设置并放置
void NodeSetIdx(cxAny pview,cxVec2f idx);

//重置到旧位置
void NodeResetPosition(cxAny pview);

//检测新索引是否可以放置
cxBool NodeIdxIsValid(cxAny pview,cxVec2f curr);

//移动成功返回true
cxBool NodeSetPosition(cxAny pview,cxVec2f idx,cxBool animate);

void NodeSetSize(cxAny pview,cxSize2f size);

void NodeInit(cxAny pview,cxAny map,cxSize2f size,cxVec2f pos,NodeType type);

void NodeSetSubType(cxAny pview,NodeSubType type);

CX_C_END

#endif
