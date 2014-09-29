//
//  Defence.c
//  Home
//
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxFollow.h>
#include <Map.h>
#include "Defence.h"
#include <Range.h>
#include "Bullet.h"

AttackActionResult DefenceAttackAction(cxAny pattacker,cxAny ptarget)
{
    CX_ASSERT_VALUE(pattacker, Node, attacker);
    CX_ASSERT_VALUE(ptarget, Node, target);
    Map map = NodeGetMap(attacker);
    
    Bullet bullet = CX_CREATE(Bullet);
    BulletInit(bullet, map, cxSize2fv(10, 10), cxViewPosition(attacker));
    BulletSetPower(bullet, NodeGetPower(attacker));
    
    cxFollow follow = cxFollowCreate(500, target);
    
    return AttackActionResultMake(bullet, follow);
}

PathRuleResult DefencePathRule(cxAny pview,FindRuleResult *fret)
{
    CX_ASSERT_THIS(pview, Defence);
    //目标未达到攻击范围
    if(!NodeIsArriveRange(this, fret->target)){
        return PathRuleResultEmpty();
    }
    return PathRuleResultMake(fret->target, NodeBindReasonAttack);
}

FindRuleResult DefenceFindRule(cxAny pview,const NodeCombined *type)
{
    CX_ASSERT_THIS(pview, Node);
    cxRange2f range = NodeGetRange(this);
    //搜索攻击范围内的目标
    FindRuleResult ret = FindRuleResultEmpty();
    ret.target = MapNearestQuery(this, *type, range);
    if(ret.target != NULL){
        ret.fd = NodeFindReasonRange;
        return ret;
    }
    return ret;
}

CX_OBJECT_TYPE(Defence, Node)
{
    
}
CX_OBJECT_INIT(Defence, Node)
{
    NodeSetType(this, NodeCombinedMake(NodeTypeDefence, NodeSubTypeNone));
    NodeSetSize(this, cxSize2iv(2, 2));
    NodeSetAttackRate(this, 0.5f);
    NodeSetRange(this, cxRange2fv(0, 11));

    NodeSetSearchOrder(this, NodeTypeAttack, NodeSubTypeNone);
    
    cxSpriteSetTextureURL(this, "bullet.json?shell.png");
    cxViewSetColor(this, cxRED);
    
    Range range = CX_CREATE(Range);
    RangeSetRange(range, NodeGetRange(this));
    cxViewAppend(this, range);
    
    SET(Node,this,PathRule, DefencePathRule);
    SET(Node,this,FindRule, DefenceFindRule);
    SET(Node,this,AttackAction, DefenceAttackAction);
}
CX_OBJECT_FREE(Defence, Node)
{
    
}
CX_OBJECT_TERM(Defence, Node)

Defence DefenceCreate(cxAny pmap,cxVec2i pos)
{
    Defence this = CX_CREATE(Defence);
    NodeInit(this, pmap, pos, true);
    NodeSearchRun(this);
    return this;
}

