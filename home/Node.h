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

    cxBool isStatic;    //node 是否可移动，移动的node不加入nodes,不参与路径搜索
    cxVec2i initIdx;    //初始化放置的位置,对于静态物不会变化
    cxBool isDie;       //是否死去

    CX_FIELD_DEF(cxAny Map);
    CX_FIELD_DEF(cxFloat Body);
    CX_FIELD_DEF(cxVec2f Index);
    CX_FIELD_DEF(NodeCombined Type);  //node组合类型,攻击范围
    CX_FIELD_DEF(cxRange2f Range);    //攻击范围
    CX_FIELD_DEF(cxSize2i Size);      //占用的格子数
    CX_FIELD_DEF(cxFloat Speed);      //移动速度
    CX_FIELD_DEF(cxFloat Power);      //攻击力,每秒的攻击力，实际效果和攻击频率(attackRate)有关系
    CX_FIELD_DEF(cxRange2i Life);     //min当前生命，max最大生命
    CX_FIELD_DEF(cxRange2f Field);    //视野范围定义了，优先攻击范围内可以到达的目标,默认值为 0 - 5
    CX_FIELD_DEF(cxInt Level);        //等级
    CX_FIELD_DEF(cxFloat AttackRate); //攻击频率
    CX_FIELD_DEF(cxFloat SearchRate); //搜索频率
    CX_FIELD_DEF(cxInt AttackNum);    //同时攻击的数量
    CX_FIELD_DEF(cxInt DirIndex);     //当前方向索引
    CX_FIELD_DEF(cxFloat DirAngle);   //方向偏转角
    cxTimer attackTimer;//攻击定时器
    //自动搜索
    cxTimer searchTimer;      //搜索用定时器
    cxInt searchIndex;
    //生命值变化
    CX_EVENT_ALLOC(onLife);
    //死亡时
    CX_EVENT_ALLOC(onDie);
    NodeSearchOrder orders;
    //目标搜索规则
    CX_METHOD_DEF(cxAny, FindRule,cxAny,const NodeCombined *);
    //路径搜索规则
    CX_METHOD_DEF(cxAny, PathRule, cxAny seacher, cxAny target);
    //是否能攻击目标
    CX_METHOD_DEF(cxBool, IsAttackTarget,cxAny attacker,cxAny target);
    //node被finder发现,返回false表示不能被攻击(谁发现了node,回答是finder)
    CX_METHOD_DEF(cxBool, Finded,cxAny node,cxAny finder);
    //方向发生变化
    CX_METHOD_DEF(void, NodeDirection,cxAny);
    //攻击一个目标 attcker攻击target bd=bind数据
    CX_METHOD_DEF(void, AttackTarget,cxAny attcker,cxAny target,cxAny bd);
    //被一个目标攻击 pview 被attacker攻击 attacktype可能是 弓箭或者node
    CX_METHOD_DEF(void, NodeAttacked,cxAny pview,cxAny attacker,AttackType type);
    //当Node MoveTo target结束时 attcker target,返回 false表示解除bind
    CX_METHOD_DEF(cxBool, MoveExit,cxAny,cxAny);
CX_OBJECT_END(Node, cxSprite)

CX_FIELD_GET(Node, cxAny, Map);

CX_FIELD_IMP(Node, NodeCombined, Type);
CX_FIELD_SET(Node, cxRange2f, Range);

CX_FIELD_GET(Node, cxSize2i, Size);
void NodeSetSize(cxAny pview,cxSize2i size);

CX_FIELD_IMP(Node, cxVec2f, Index);
CX_FIELD_IMP(Node, cxFloat, Speed);
CX_FIELD_IMP(Node, cxFloat, Power);
CX_FIELD_IMP(Node, cxFloat, Body);
CX_FIELD_IMP(Node, cxRange2i, Life);
CX_FIELD_SET(Node, cxRange2f, Field);
CX_FIELD_IMP(Node, cxInt, Level);
CX_FIELD_IMP(Node, cxFloat, AttackRate);
CX_FIELD_IMP(Node, cxFloat, SearchRate);
CX_FIELD_IMP(Node, cxInt, AttackNum);
CX_FIELD_IMP(Node, cxInt, DirIndex);

CX_INLINE cxVec2i NodeGetIntIndex(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, Node);
    return cxVec2iv(this->Index.x, this->Index.y);
}

//获取视野，从body后开始计算
CX_INLINE cxRange2f NodeGetField(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, Node);
    cxRange2f range = this->Field;
    range.max += NodeGetBody(this);
    return range;
}

//获取攻击范围，和node' Body有关系
CX_INLINE cxRange2f NodeGetRange(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, Node);
    cxRange2f range = this->Range;
    range.max += NodeGetBody(this);
    return range;
}

CX_FIELD_GET(Node, cxFloat, DirAngle);
void NodeSetDirAngle(cxAny pview,cxFloat angle);

void NodeAddLife(cxAny pview,cxInt life);

//获取两个node之间的有效果距离，减去各自的body
cxFloat NodeFightDistance(cxAny src,cxAny dst);

//检测是否在attacker的作战范围内
cxBool NodeArriveFightRange(cxAny attacker,cxAny target);

//启动攻击定时器
void NodeStartupAttackTimer(cxAny pview);

//使用点集合移动移动到目标,移动结束触发 MoveExit 回调
void NodeMovingToTarget(cxAny pview,cxAny target, cxAnyArray points);

//node被攻击的默认处理程序
void NodeAttacked(cxAny pview,cxAny attacker,AttackType type);

//攻击一个目标
void NodeAttackTarget(cxAny attacker,cxAny target,AttackType type);

//添加搜索顺序
void NodeSetSearchOrder(cxAny pview,NodeType type,NodeSubType subType);
//清空搜索顺序
void NodeSearchOrderClear(cxAny pview);

//朝向target
void NodeFaceTarget(cxAny pview,cxAny target);

//根据网格坐标设置位置，与左下角格子中心为铆合点
void NodeInitIndex(cxAny pview,cxVec2i idx);

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

//初始化node
void NodeInit(cxAny pview,cxAny map,cxVec2i idx,cxBool isStatic);

CX_C_END

#endif
