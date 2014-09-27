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

static void AttackAttackTarget(cxAny pview,cxAny target)
{
    CX_ASSERT_THIS(pview, Attack);
    NodeAttackTarget(this, target, AttackTypeDirect);
}

static cxAny AttackFindTarget(cxAny pview,cxAny target)
{
    CX_ASSERT_THIS(pview, Attack);
    Map map = NodeMap(this);
    //已达到攻击范围
    if(CX_METHOD_GET(false, this->Node.IsAttackTarget, this,target)){
        return target;
    }
    //搜索目标路径成功,移动到可攻击位置
    if(MapSearchPath(this, target)){
        NodeMovingToTarget(this, target, MapSearchPoints(map));
        return target;
    }
    //使用线段搜索法搜索距离目标最近的一个阻挡物
    Node block = MapSegmentQuery(this, target, NodeCombinedMake(NodeTypeBlock, NodeSubTypeNone));
    if(block == NULL){
        return NULL;
    }
    //搜索阻挡物失败
    if(MapSearchPath(this, block)){
        NodeMovingToTarget(this, block, MapSearchPoints(map));
        return block;
    }
    return NULL;
}

CX_OBJECT_TYPE(Attack, Node)
{
    
}
CX_OBJECT_INIT(Attack, Node)
{
    NodeSetType(this, NodeTypeAttack);
    NodeSetSearchOrder(this, NodeTypeDefence, NodeSubTypeNone);
    NodeSetBody(this, 0.5f);
    NodeSetSize(this, cxSize2iv(1, 1));
    NodeSetSpeed(this, 100);
    NodeSetField(this, cxRange2fv(0, 20));
    //近身攻击00
    NodeSetRange(this, cxRange2fv(0, 0));
    NodeSetAttackRate(this, 0.1f);
    cxSpriteSetTextureURL(this, "bullet.json?shell.png");
    cxViewSetColor(this, cxRED);
    
    CX_METHOD_SET(this->Node.FindTarget, AttackFindTarget);
    CX_METHOD_SET(this->Node.AttackTarget, AttackAttackTarget);
}
CX_OBJECT_FREE(Attack, Node)
{
    
}
CX_OBJECT_TERM(Attack, Node)

Attack AttackCreate(cxAny map,cxVec2i pos)
{
    Attack this = CX_CREATE(Attack);
    NodeInit(this, map, pos, false);
    NodeSearchRun(this);
    return this;
}