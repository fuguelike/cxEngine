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
    cxSprite array;     //Test
    cxAny map;
    cxVec2f index;      //更精确的浮点格子坐标,根据node当前位置计算
    cxBool isStatic;    //node 是否可移动，移动的node不加入nodes,不参与路径搜索
    cxVec2i initIdx;    //初始化放置的位置,对于静态物不会变化
    cxSize2i size;      //占用的格子数
    cxBool isDie;       //是否死去
    NodeCombined type;  //node组合类型,攻击范围
    cxRange2f range;    //攻击范围
    cxFloat body;       //可攻击半径
    cxFloat attackRate; //攻击频率
    cxFloat speed;      //移动速度
    cxFloat power;      //攻击力,每秒的攻击力，实际效果和攻击频率(attackRate)有关系
    cxRange2i life;     //min当前生命，max最大生命
    cxRange2f field;    //视野范围定义了，优先攻击范围内可以到达的目标,默认值为 0 - 5
    cxInt level;        //等级
    //自动搜索
    cxTimer searchTimer;      //搜索用定时器
    cxInt searchIndex;
    //生命值变化
    CX_EVENT_ALLOC(onLife);
    //死亡时
    CX_EVENT_ALLOC(onDie);
    NodeSearchOrder orders;
    //记录了上一个网格活动位置，初始和initIdx相等
    cxVec2i activeIdx;
    //是否能攻击目标
    CX_METHOD_DEF(cxBool, IsAttackTarget,cxAny attacker,cxAny target);
    //搜索到目标,返回bind哪个目标 seacher,target
    CX_METHOD_DEF(cxAny, FindTarget, cxAny seacher, cxAny target);
    //node被finder发现,返回false表示不能被攻击(谁发现了node,回答是finder)
    CX_METHOD_DEF(cxBool, NodeFinded,cxAny node,cxAny finder);
    cxInt attackNum;    //同时攻击的数量
    //方向发生变化
    cxInt   dirIndex;    //当前方向索引
    cxFloat dirAngle;    //方向偏转角
    CX_METHOD_DEF(void, NodeDirection,cxAny);
    //攻击一个目标 attcker攻击target
    CX_METHOD_DEF(void, AttackTarget,cxAny attcker,cxAny target);
    //被一个目标攻击 pview 被attacker攻击 attacktype可能是 弓箭或者node
    CX_METHOD_DEF(void, NodeAttacked,cxAny pview,cxAny attacker,AttackType type);
    //当Node MoveTo target结束时 attcker target,返回 false表示解除bind
    CX_METHOD_DEF(cxBool, MoveExit,cxAny,cxAny);
CX_OBJECT_END(Node, cxSprite)

//获取两个node之间的有效果距离，减去各自的body
cxFloat NodeDistance(cxAny src,cxAny dst);

//检测是否在attacker的作战范围内
cxBool NodeIsAttackTarget(cxAny attacker,cxAny target);

//启动攻击定时器
void NodeStartupAttackTimer(cxAny pview);

//使用点集合移动移动到目标,移动结束触发 MoveExit 回调
void NodeMovingToTarget(cxAny pview,cxAny target, cxAnyArray points);

//node被攻击的默认处理程序
void NodeAttacked(cxAny pview,cxAny attacker,AttackType type);

//攻击一个目标
void NodeAttackTarget(cxAny attacker,cxAny target,AttackType type);

//设置node方向并触发事件
void NodeSetDirAngle(cxAny pview,cxFloat angle);

//添加搜索顺序
void NodeSetSearchOrder(cxAny pview,NodeType type,NodeSubType subType);
//清空搜索顺序
void NodeSearchOrderClear(cxAny pview);

//朝向target
void NodeFaceTarget(cxAny pview,cxAny target);

//设置视野范围
void NodeSetField(cxAny pview,cxRange2f field);
cxRange2f NodeField(cxAny pview);

//根据网格坐标设置位置，与左下角格子中心为铆合点
void NodeSetIndex(cxAny pview,cxVec2i idx);

void NodeSetSize(cxAny pview,cxSize2i size);
cxSize2i NodeSize(cxAny pview);
//设置生命 等级 攻击力
void NodeSetLife(cxAny pview,cxInt life);
void NodeAddLife(cxAny pview,cxInt life);
cxRange2i NodeLife(cxAny pview);

void NodeSetLevel(cxAny pview,cxInt level);
cxInt NodeLevel(cxAny pview);

void NodeSetBody(cxAny pview,cxFloat body);
cxFloat NodeBody(cxAny pview);

void NodeSetPower(cxAny pview,cxFloat power);
cxFloat NodePower(cxAny pview);

void NodeSetSpeed(cxAny pview,cxFloat speed);
cxFloat NodeSpeed(cxAny pview);
//
void NodeSetType(cxAny pview,NodeType type);
void NodeSetSubType(cxAny pview,NodeSubType subType);
//设置获取攻击频率
cxFloat NodeAttackRate(cxAny pview);
void NodeSetAttackRate(cxAny pview,cxFloat rate);

//设置攻击范围
void NodeSetRange(cxAny pview,cxRange2f range);
cxRange2f NodeRange(cxAny pview);

//返回关联的map对象
cxAny NodeMap(cxAny pview);

//如果node是静态
cxBool NodeIsStatic(cxAny pview);

//启动搜索程序
void NodeSearchRun(cxAny pview);

//检测node是否死亡
cxBool NodeCheckDie(cxAny pview);

//暂停搜索
void NodePauseSearch(cxAny pview);

//重新启动搜索
void NodeResumeSearch(cxAny pview);

//返回精确的网格坐标
cxVec2f NodeFloatIndex(cxAny pview);

//初始化node
void NodeInit(cxAny pview,cxAny map,cxVec2i idx,cxBool isStatic);

CX_C_END

#endif
