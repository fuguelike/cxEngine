//
//  Longer.c
//  cxCore
//
//  Created by xuhua on 9/27/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <Bullet.h>
#include <actions/cxFollow.h>
#include <actions/cxScale.h>
#include <Map.h>
#include "Longer.h"

AttackActionResult LongerAttackAction(cxAny pattacker,cxAny ptarget)
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

FindRuleResult LongerFindRule(cxAny pview,const NodeCombined *type)
{
    return AttackFindRule(pview, type);
}

//一次攻击完成
static void LongerAttackOnce(cxAny pattacker,cxAny ptarget)
{
    cxViewSetScale(pattacker, cxVec2fv(1, 1));
}

CX_OBJECT_TYPE(Longer, Attack)
{
    
}
CX_OBJECT_INIT(Longer, Attack)
{
    NodeSetRange(this, cxRange2fv(0, 5));
    NodeSetAttackRate(this, 0.5f);
    SET(Node, this, FindRule, LongerFindRule);
    SET(Node, this, AttackAction, LongerAttackAction);
    SET(Node, this, AttackOnce, LongerAttackOnce);
    cxViewSetColor(this, cxORANGE);
}
CX_OBJECT_FREE(Longer, Attack)
{

}
CX_OBJECT_TERM(Longer, Attack)

void LongerInit(cxAny pview, cxAny pmap,cxVec2i pos)
{
    CX_ASSERT_THIS(pview, Longer);
    AttackInit(this, pmap, pos);
}

Longer LongerCreate(cxAny pmap,cxVec2i pos)
{
    Longer this = CX_CREATE(Longer);
    LongerInit(this, pmap, pos);
    return this;
}