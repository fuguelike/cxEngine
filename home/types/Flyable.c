//
//  Flyable.c
//  cxCore
//
//  Created by xuhua on 9/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxScale.h>
#include <Map.h>
#include <Node.h>
#include <Bullet.h>
#include "Flyable.h"

AttackActionResult FlyableAttackAction(cxAny pattacker,cxAny ptarget)
{
    CX_ASSERT_VALUE(pattacker, Node, attacker);
    CX_ASSERT_VALUE(ptarget, Node, target);
    Map map = NodeGetMap(attacker);
    
    //攻击武器动画
    Bullet bullet = CX_CREATE(Bullet);
    BulletInit(bullet, map, cxSize2fv(10, 10), cxViewGetPosition(attacker));
    BulletSetPower(bullet, NodeGetPower(attacker));
    
    //自身攻击动画
    cxScale scale = cxScaleCreate(NodeGetAttackRate(attacker), cxVec2fv(1.2f, 1.2f));
    
    return AAMake(bullet, scale);
}

PathRuleResult FlyablePathRule(cxAny pview,FindRuleResult *fret)
{
    CX_ASSERT_THIS(pview, Attack);
    Map map = NodeGetMap(this);
    //飞行单位直接到达，无需寻路
    MapCleanPoints(map);
    cxVec2f sidx = NodeGetIndex(this);
    cxVec2f didx = NodeGetIndex(fret->target);
    //先加目标点，在加入移动动画时反向取
    MapAppendPoint(map, didx);
    MapAppendPoint(map, sidx);
    return PRMake(fret->target, NodeBindReasonMove);
}

FindRuleResult FlyableFindRule(cxAny pview,const NodeCombined *type)
{
    return AttackFindRule(pview, type);
}

//一次攻击完成
static void FlyableAttackOnce(cxAny pattacker,cxAny ptarget)
{
    cxViewSetScale(pattacker, cxVec2fv(1, 1));
}

CX_OBJECT_TYPE(Flyable, Attack)
{
    
}
CX_OBJECT_INIT(Flyable, Attack)
{
    NodeSetRange(this, cxRange2fv(0, 4));
    NodeSetAttackRate(this, 0.5f);
    NodeSetSpeed(this, 200);
    cxViewSetColor(this, cxYELLOW);
    CX_SET(Node, this, PathRule, FlyablePathRule);
    CX_SET(Node, this, FindRule, FlyableFindRule);
    CX_SET(Node, this, AttackAction, FlyableAttackAction);
    CX_SET(Node, this, AttackOnce, FlyableAttackOnce);
}
CX_OBJECT_FREE(Flyable, Attack)
{

}
CX_OBJECT_TERM(Flyable, Attack)

void FlyableInit(cxAny pview, cxAny pmap,cxVec2i idx)
{
    CX_ASSERT_THIS(pview, Flyable);
    AttackInit(this, pmap, idx);
}

Flyable FlyableCreate(cxAny pmap,cxVec2i idx)
{
    Flyable this = CX_CREATE(Flyable);
    FlyableInit(this, pmap, idx);
    return this;
}