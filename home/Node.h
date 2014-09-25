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
    NodeCombined type;  //node组合类型
    NodeState state;    //当前状态
    cxRange2f attackRange;  //攻击范围
    cxRange2f searchRange;  //搜索范围
    cxFloat body;       //可攻击半径
    cxFloat attackRate; //攻击频率
    cxFloat speed;      //移动速度
    cxFloat power;       //攻击力,每秒的攻击力，实际效果和攻击频率(attackRate)有关系
    cxRange2i life;     //min当前生命，max最大生命
    cxInt level;        //等级
    cxTimer searchTimer;      //搜索用定时器
    cxInt searchIndex;
    cxTimer attackTimer;      //攻击用定时器
    //生命值变化
    CX_EVENT_ALLOC(onLife);
    //死亡时
    CX_EVENT_ALLOC(onDie);
    NodeSearchOrder orders;
    //搜索到目标,返回bind哪个目标 seacher,target
    CX_METHOD_DEF(cxAny, Finded, cxAny seacher, cxAny target,cxBool *);
    cxInt attackNum;    //同时攻击的数量
    //方向发生变化
    cxInt   dirIndex;    //当前方向索引
    cxFloat dirAngle;    //方向偏转角
    CX_METHOD_DEF(void, Direction,cxAny);
    //攻击一个目标 attcker攻击target
    CX_METHOD_DEF(void, Attack,cxAny attcker,cxAny target);
    //被一个目标攻击 pview 被attacker攻击 attacktype可能是 弓箭或者node
    CX_METHOD_DEF(void, Attacked,cxAny pview,cxAny attacker,AttackType type);
    //当Node MoveTo target结束时 attcker target,返回 false表示解除bind
    CX_METHOD_DEF(cxBool, MoveExit,cxAny,cxAny);
CX_OBJECT_END(Node, cxSprite)

//使用点集合移动到bind的node,移动结束触发 MoveExit 回调
void NodeMoveTo(cxAny pview,cxAnyArray points);

//node被攻击
void NodeAttacked(cxAny pview,cxAny attacker,AttackType type);

//攻击一个目标
void NodeAttackTarget(cxAny attacker,cxAny target,AttackType type);

//设置node方向并触发事件
void NodeSetDirAngle(cxAny pview,cxFloat angle);

//添加搜索顺序
void NodeSearchOrderAdd(cxAny pview,NodeType type,NodeSubType subType);
//清空搜索
void NodeSearchOrderClear(cxAny pview);

//处死Node
void NodeMomentDie(cxAny pview);

//离idx最近的node本地坐标点
cxVec2f NodeNearestPoint(cxAny pview,cxVec2f idx);

//攻击者是否到达目标的攻击范围
cxBool NodeArriveAttack(cxAny pattacker,cxAny ptarget);

//设置生命 等级 攻击力
void NodeSetLife(cxAny pview,cxInt life);
void NodeAddLife(cxAny pview,cxInt life);
void NodeSetLevel(cxAny pview,cxInt level);
void NodeSetBody(cxAny pview,cxFloat body);
void NodeSetPower(cxAny pview,cxFloat power);
void NodeSetSpeed(cxAny pview,cxFloat speed);
void NodeSetType(cxAny pview,NodeType type);
void NodeSetSubType(cxAny pview,NodeSubType subType);
//获取攻击力
cxFloat NodePower(cxAny pview);

//设置攻击范围
void NodeSetAttackRange(cxAny pview,cxRange2f range);

//设置搜索范围
void NodeSetSearchRange(cxAny pview,cxRange2f range);

//设置攻击频率
void NodeSetAttackRate(cxAny pview,cxFloat rate);

cxAny NodeMap(cxAny pview);

//节点死亡
cxBool NodeIsDie(cxAny pview);

//设置状态
void NodeSetState(cxAny pview,NodeState state);

//暂停搜索
void NodePauseSearch(cxAny pview);

//重新启动搜索
void NodeResumeSearch(cxAny pview);

//启动搜索定时器
void NodeSearchRun(cxAny pview);

//暂停攻击
void NodePauseAttack(cxAny pview);

//重新启动攻击
void NodeResumeAttack(cxAny pview);

//启动攻击定时器
void NodeAttackRun(cxAny pview);

cxRange2i NodeLife(cxAny pview);

cxBool NodeHasPoint(cxAny pview,cxVec2i idx);

cxInt NodeLevel(cxAny pview);

cxVec2i NodeIndex(cxAny pview);

cxSize2i NodeSize(cxAny pview);

//获取Node当前位置索引
cxVec2f NodePosIndex(cxAny pview);

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

void NodeInit(cxAny pview,cxAny map,cxSize2f size,cxVec2f idx);

CX_C_END

#endif
