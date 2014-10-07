//
//  Attack.c
//  Home
//
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//
#include <Move.h>
#include <Map.h>
#include <Range.h>
#include <actions/cxScale.h>
#include "Attack.h"

PathRuleResult AttackPathRule(cxAny pview,const FindRuleResult *fret)
{
    CX_ASSERT_THIS(pview, Attack);
    //搜索目标路径成功,移动到可攻击位置
    if(MapSearchPath(this, fret->target)){
        return PRMake(fret->target, NodeBindReasonMove);
    }
    //使用线段搜索法搜索距离目标最近的一个阻挡物
    Node block = MapSegmentQuery(this, fret->target, NodeCombinedMake(NodeTypeBlock, NodeSubTypeNone));
    if(block != NULL && MapSearchPath(this, block)){
        return PRMake(block, NodeBindReasonMove);
    }
    return PREmpty();
}

FindRuleResult AttackFindRule(cxAny pview,const NodeCombined *type)
{
    CX_ASSERT_THIS(pview, Node);
    FindRuleResult ret = FREmpty();
    //搜索攻击范围内的目标
    cxRange2f range = NodeGetRange(this);
    ret.target = MapNearestQuery(this, *type, range);
    if(ret.target != NULL){
        ret.fd = NodeFindReasonRange;
        return ret;
    }
    //搜索视野范围内的目标
    cxRange2f field = NodeGetField(this);
    ret.target = MapNearestQuery(this, *type, field);
    if(ret.target != NULL){
        ret.fd = NodeFindReasonField;
        return ret;
    }
    //全屏搜索
    ret.target = MapNearestQuery(this, *type, MAX_RANGE);
    if(ret.target != NULL){
        ret.fd = NodeFindReasonAll;
        return ret;
    }
    return ret;
}

ActionResult AttackAttackAction(cxAny pattacker,cxAny ptarget)
{
    CX_ASSERT_VALUE(pattacker, Node, attacker);
    CX_ASSERT_TYPE(ptarget, Node);
    cxScale scale = cxScaleCreate(NodeGetAttackRate(attacker), cxVec2fv(1.2f, 1.2f));
    return AAMake(NULL, scale);
}

static void AttackAttackOnce(cxAny pattacker,cxAny ptarget)
{
    cxViewSetScale(pattacker, cxVec2fv(1, 1));
}

CX_OBJECT_TYPE(Attack, Node)
{
    
}
CX_OBJECT_INIT(Attack, Node)
{
    NodeSetType(this, NodeCombinedMake(NodeTypeAttack, NodeSubTypeNone));
    NodeSetSearchOrder(this, NodeTypeDefence, NodeSubTypeNone);
    NodeSetSize(this, cxSize2iv(1, 1));
    NodeSetSpeed(this, 100);
    NodeSetField(this, cxRange2fv(0, 10));
    NodeSetRange(this, cxRange2fv(0, 0));
    NodeSetAttackRate(this, 0.2f);
    cxSpriteSetTextureURL(this, "bullet.json?shell.png");
    cxViewSetColor(this, cxBLUE);
    
    CX_SET(Node,this,PathRule, AttackPathRule);
    CX_SET(Node,this,FindRule, AttackFindRule);
    CX_SET(Node,this,AttackAction, AttackAttackAction);
    CX_SET(Node,this,AttackOnce, AttackAttackOnce);
    
//    Range range = CX_CREATE(Range);
//    RangeSetRange(range, NodeGetField(this));
//    cxViewAppend(this, range);
}
CX_OBJECT_FREE(Attack, Node)
{
    
}
CX_OBJECT_TERM(Attack, Node)

void AttackInit(cxAny pview, cxAny pmap,cxVec2i idx)
{
    CX_ASSERT_THIS(pview, Attack);
    NodeInit(this, pmap, idx, false);
    NodeSearchRun(this);
}

Attack AttackCreate(cxAny pmap,cxVec2i idx)
{
    Attack this = CX_CREATE(Attack);
    AttackInit(this, pmap, idx);
    return this;
}