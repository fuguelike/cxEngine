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
#include <views/cxLabelTTF.h>
#include "Define.h"

CX_C_BEGIN

typedef struct {
    cxAny target;
    NodeBindReason bd;
}PathRuleResult;
#define PRMake(_t_,_d_)     (PathRuleResult){_t_,_d_}
#define PREmpty()           PRMake(NULL,NodeBindReasonNone)

typedef struct {
    cxAny target;
    NodeFindReason fd;
}FindRuleResult;
#define FRMake(_t_,_d_)     (FindRuleResult){_t_,_d_}
#define FREmpty()           FRMake(NULL,NodeFindReasonNone)

typedef struct {
    cxAny bullet;
    cxAny action;
}ActionResult;
#define AAMake(_b_,_a_)     (ActionResult){_b_,_a_}
#define AAEmpty()           AAMake(NULL,NULL)

CX_OBJECT_DEF(Node, cxSprite)

    cxLabelTTF lifeTTF;     //Test
    cxSprite array;     //Test

    cxBool isDie;                       //是否死去
    CX_FIELD_DEF(cxBool IsStatic);      //是否是静态物体，静态物体将被加入a*搜索列表中
    CX_FIELD_DEF(cxVec2i InitIndex);    //初始化放置的位置,对于静态物不会变化
    CX_FIELD_DEF(cxFloat Body);         //用于确定单位停留在目标的附近哪个位置,默认值为0.5格
    CX_FIELD_DEF(cxAny Map);            //关联的map对象
    CX_FIELD_DEF(cxVec2f Index);        //精确的网格索引0 - 39.99999f
    CX_FIELD_DEF(NodeCombined Type);    //node组合类型,攻击范围
    CX_FIELD_DEF(cxRange2f Range);      //攻击范围
    CX_FIELD_DEF(cxSize2i Size);        //占用的格子数
    CX_FIELD_DEF(cxFloat Speed);        //移动速度
    CX_FIELD_DEF(cxFloat Power);        //攻击力,每秒的攻击力，实际效果和攻击频率(attackRate)有关系
    CX_FIELD_DEF(cxRange2i Life);       //min当前生命，max最大生命
    CX_FIELD_DEF(cxRange2f Field);      //视野范围定义了，优先攻击范围内可以到达的目标,默认值为 0 - 5
    CX_FIELD_DEF(cxInt Level);          //等级
    CX_FIELD_DEF(cxFloat AttackRate);   //攻击频率
    CX_FIELD_DEF(cxFloat SearchRate);   //搜索频率
    CX_FIELD_DEF(cxInt AttackNum);      //同时攻击的数量

    CX_FIELD_DEF(cxInt DirIndex);       //当前方向索引 0-7 一共8个方向 从0度到360度
    CX_FIELD_DEF(cxFloat DirAngle);     //方向偏转角 实际与8方向的偏移角
    CX_FIELD_DEF(NodeState State);      //当前状态
    CX_EVENT_ALLOC(onState);            //方向或者状态发生改变,例如静止下方向改变，或者移动，攻击,此事件用来控制纹理动画

    cxVec2i prevIdx;                    //上一个格子坐标
    CX_EVENT_ALLOC(onIndex);            //到达某个格子

    cxTimer attackTimer;                //攻击定时器
    cxTimer searchTimer;                //搜索用定时器
    cxInt searchIndex;

    CX_EVENT_ALLOC(onLife);             //生命值变化
    CX_EVENT_ALLOC(onDie);              //死亡时

    NodeSearchOrder orders;             //搜索顺序
    CX_METHOD_DEF(void, AttackOnce,cxAny attacker,cxAny target);                    //node一次攻击动画结束时调用
    CX_METHOD_DEF(FindRuleResult, FindRule,cxAny,const NodeCombined *);             //目标搜索规则
    CX_METHOD_DEF(PathRuleResult, PathRule, cxAny seacher,const FindRuleResult *fr);//路径搜索规则
    CX_METHOD_DEF(cxBool, Finded,cxAny node,cxAny finder);                          //node是否能被finder发现
    CX_METHOD_DEF(void, NodeAttacked,cxAny pview,cxAny attacker,AttackType type);   //被一个目标攻击 pview 被attacker攻击
    CX_METHOD_DEF(ActionResult, AttackAction,cxAny attacker,cxAny target);          //创建一个攻击动画,动画结时攻击目标
CX_OBJECT_END(Node, cxSprite)

CX_FIELD_GET(Node, NodeState, State);
CX_INLINE void NodeSetState(cxAny pthis,const NodeState state)
{
    CX_ASSERT_THIS(pthis, Node);
    CX_RETURN(this->State == state);
    this->State = state;
    CX_EVENT_FIRE(this, onState);
}

CX_FIELD_IMP(Node, cxBool, IsStatic);
CX_FIELD_IMP(Node, cxVec2i, InitIndex);
CX_FIELD_GET(Node, cxAny, Map);
CX_FIELD_IMP(Node, cxFloat, Body);
CX_FIELD_IMP(Node, NodeCombined, Type);
CX_FIELD_IMP(Node, cxRange2f, Range);

CX_FIELD_GET(Node, cxSize2i, Size);
void NodeSetSize(cxAny pview,cxSize2i size);

CX_FIELD_IMP(Node, cxVec2f, Index);
CX_FIELD_IMP(Node, cxFloat, Speed);

//设置或者获取攻击力
CX_FIELD_SET(Node, cxFloat, Power);
CX_INLINE cxFloat NodeGetPower(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, Node);
    return this->Power;
}

CX_FIELD_GET(Node, cxRange2i, Life);
CX_INLINE void NodeSetLife(cxAny pthis,cxRange2i life)
{
    CX_ASSERT_THIS(pthis, Node);
    this->Life = life;
    CX_EVENT_FIRE(this, onLife);
}
//添加或者较少node生命值
CX_INLINE void NodeAddLife(cxAny pthis,cxInt life)
{
    CX_ASSERT_THIS(pthis, Node);
    this->Life.min += life;
    CX_EVENT_FIRE(this, onLife);
}

CX_FIELD_IMP(Node, cxRange2f, Field);
CX_FIELD_IMP(Node, cxInt, Level);
CX_FIELD_IMP(Node, cxFloat, AttackRate);
CX_FIELD_IMP(Node, cxFloat, SearchRate);
CX_FIELD_IMP(Node, cxInt, AttackNum);
CX_FIELD_IMP(Node, cxInt, DirIndex);
//获取精确索引下node范围
CX_INLINE cxBox4f NodeGetBox(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, Node);
    cxBox4f box;
    cxFloat w2 = this->Size.w / 2.0f;
    cxFloat h2 = this->Size.h / 2.0f;
    box.l = this->Index.x - w2;
    box.r = this->Index.x + w2;
    box.t = this->Index.y + h2;
    box.b = this->Index.y - h2;
    return box;
}
//获取node中心所在网格的的中心网格坐标
CX_INLINE cxVec2f NodeCenterIdx(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, Node);
    return cxVec2fv((cxInt)(this->Index.x) + 0.5f, (cxInt)(this->Index.y) + 0.5f);
}

CX_INLINE cxVec2i NodeGetIntIndex(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, Node);
    return cxVec2iv(this->Index.x, this->Index.y);
}

CX_FIELD_GET(Node, cxFloat, DirAngle);

void NodeSetDirAngle(cxAny pview,cxFloat angle);

//获取node间的格子距离
CX_INLINE cxFloat NodeDistance(cxAny src,cxAny dst)
{
    CX_ASSERT_VALUE(src, Node, snode);
    CX_ASSERT_VALUE(dst, Node, dnode);
    cxVec2f sidx = NodeGetIndex(snode);
    cxVec2f didx = NodeGetIndex(dnode);
    return kmVec2DistanceBetween(&sidx, &didx);
}

//检测网格坐标是否选在node范围内
cxBool NodeHited(cxAny pview,cxVec2f index);

//检测是到达攻击范围,是否加入attacker或者target body计算
cxBool NodeIsArriveRange(cxAny attacker,cxAny target);

//启动攻击定时器
void NodeStartupAttackTimer(cxAny pview);

//使用点集合移动
void NodeMoveToPosition(cxAny pview,cxAnyArray points);

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

//node之间的距离
cxFloat NodeDistance(cxAny src,cxAny dst);

//朝向target
void NodeFaceTarget(cxAny pview,cxAny target);

//重置node位置为初始位置
void NodeResetIndex(cxAny pview);

//使用自身pos位置更新index,不能更新返回false
cxBool NodeUpdateInitIndex(cxAny pview);

//根据网格坐标设置位置，与左下角格子中心为铆合点
void NodeUpdateIndex(cxAny pview,cxVec2i idx);

//从精确的网格坐标转换成基于node左下角格子坐标,此坐标一般用来判断位置是否存在其他的node
cxVec2i NodeIndexToInitIndex(cxAny pview,cxVec2f idx);
//从当前精确网格坐标获取基于node左下角的整形坐标
cxVec2i NodeGetInitIndexUseIndex(cxAny pview);

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
