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
#include "Attack.h"

static void AttackAttackTarget(cxAny pview,cxAny target,cxAny bd)
{
    CX_ASSERT_THIS(pview, Attack);
    NodeAttackTarget(this, target, AttackTypeDirect);
}

PathRuleResult AttackPathRule(cxAny pview,cxAny target)
{
    CX_ASSERT_THIS(pview, Attack);
    //搜索目标路径成功,移动到可攻击位置,搜索视野范围内的路径
    if(MapSearchPath(this, target)){
        //move to target
        return PathRuleResultMake(target, NodeBindReasonMove);
    }
    //使用线段搜索法搜索距离目标最近的一个阻挡物
    Node block = MapSegmentQuery(this, target, NodeCombinedMake(NodeTypeBlock, NodeSubTypeNone));
    if(block != NULL && MapSearchPath(this, block)){
        //move to block
        return PathRuleResultMake(block, NodeBindReasonMove);
    }
    return PathRuleResultEmpty();
}

cxAny AttackFindRule(cxAny pview,const NodeCombined *type)
{
    CX_ASSERT_THIS(pview, Node);
    Node target = NULL;
    //搜索攻击范围内的目标
    cxRange2f range = NodeGetRange(this);
    if(target == NULL){
        target = MapNearestQuery(this, *type, range);
    }
    //搜索视野范围内的目标
    cxRange2f field = NodeGetField(this);
    if(target == NULL){
        target = MapNearestQuery(this, *type, field);
    }
    //全屏搜索
    if(target == NULL){
        target = MapNearestQuery(this, *type, MAX_RANGE);
    }
    return target;
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
    NodeSetRange(this, cxRange2fv(0, 1));
    NodeSetAttackRate(this, 0.2f);
    cxSpriteSetTextureURL(this, "bullet.json?shell.png");
    cxViewSetColor(this, cxBLUE);
    
    CX_METHOD_SET(this->Node.PathRule, AttackPathRule);
    CX_METHOD_SET(this->Node.AttackTarget, AttackAttackTarget);
    CX_METHOD_SET(this->Node.FindRule, AttackFindRule);
    
//    Range range = CX_CREATE(Range);
//    RangeSetRange(range, NodeGetField(this));
//    cxViewAppend(this, range);
}
CX_OBJECT_FREE(Attack, Node)
{
    
}
CX_OBJECT_TERM(Attack, Node)

void AttackInit(cxAny pview, cxAny pmap,cxVec2i pos)
{
    CX_ASSERT_THIS(pview, Attack);
    NodeInit(this, pmap, pos, false);
    NodeSearchRun(this);
}

Attack AttackCreate(cxAny pmap,cxVec2i pos)
{
    Attack this = CX_CREATE(Attack);
    AttackInit(this, pmap, pos);
    return this;
}