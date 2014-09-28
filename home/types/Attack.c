//
//  Attack.c
//  Home
//
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//
#include <Move.h>
#include <Map.h>
#include "Attack.h"

static void AttackAttackTarget(cxAny pview,cxAny target,cxAny bd)
{
    CX_ASSERT_THIS(pview, Attack);
    NodeAttackTarget(this, target, AttackTypeDirect);
}

cxAny AttackPathRule(cxAny pview,cxAny target)
{
    CX_ASSERT_THIS(pview, Attack);
    Map map = NodeGetMap(this);
    //已达到攻击范围
    if(CX_METHOD_GET(false, this->Node.IsAttackTarget, this,target)){
        return target;
    }
    //搜索目标路径成功,移动到可攻击位置,搜索视野范围内的路径
    cxRange2f field = NodeGetField(this);
    if(MapSearchPath(this, target, field.max)){
        NodeMovingToTarget(this, target, MapSearchPoints(map));
        return target;
    }
//    //移动到最后目标点
//    if(MapSearchPathPoint(map, NodeGetIntIndex(this), map->NearstIdx, field.max)){
//        NodeMovingToTarget(this, target, MapSearchPoints(map));
//        return target;
//    }
    //使用线段搜索法搜索距离目标最近的一个阻挡物
    Node block = MapSegmentQuery(this, target, NodeCombinedMake(NodeTypeBlock, NodeSubTypeNone));
    if(block != NULL && MapSearchPath(this, block, field.max)){
        NodeMovingToTarget(this, block, MapSearchPoints(map));
        return block;
    }
    return NULL;
}

cxAny AttackFindRule(cxAny pview,const NodeCombined *type)
{
    CX_ASSERT_THIS(pview, Node);
    cxRange2f range = NodeGetRange(this);
    cxRange2f field = NodeGetField(this);
    Node target = NULL;
    //搜索攻击范围内可以攻击的目标
    if(target == NULL){
        target = MapNearestQuery(this, *type, range, false);
    }
    //搜索视野范围内可以到达的目标
    if(target == NULL){
        target = MapNearestQuery(this, *type, field, true);
    }
    //搜索视野范围内最近的目标
    if(target == NULL){
        target = MapNearestQuery(this, *type, field, false);
    }
    //全屏搜索
    if(target == NULL){
        target = MapNearestQuery(this, *type, MAX_RANGE, false);
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
    NodeSetBody(this, 0.3f);
    cxSpriteSetTextureURL(this, "bullet.json?shell.png");
    cxViewSetColor(this, cxBLUE);
    
    CX_METHOD_SET(this->Node.PathRule, AttackPathRule);
    CX_METHOD_SET(this->Node.AttackTarget, AttackAttackTarget);
    CX_METHOD_SET(this->Node.FindRule, AttackFindRule);
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