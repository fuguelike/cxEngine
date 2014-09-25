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

static cxAny AttackFindTarget(cxAny pview,cxAny target,cxBool *isAttack)
{
    CX_ASSERT_THIS(pview, Attack);
    Map map = NodeMap(this);
    PathResult ret = MapSearchPath(this, target);
    //搜索目标路径成功
    if(ret.success){
        NodeMoveTo(this, MapSearchPoints(map));
        return target;
    }
    //失败获取阻挡物
    Node block = ret.block;
    if(block == NULL){
        return NULL;
    }
    ret = MapSearchPath(this, block);
    //搜索阻挡物失败
    if(!ret.success){
        return NULL;
    }
    //移动到阻挡物
    NodeMoveTo(this, MapSearchPoints(map));
    return block;
}

CX_OBJECT_TYPE(Attack, Node)
{
    
}
CX_OBJECT_INIT(Attack, Node)
{
    NodeSetType(this, NodeTypeAttack);
    NodeSearchOrderAdd(this, NodeTypeDefence, NodeSubTypeNone, MAX_RANGE);
    NodeSetBody(this, 0.3f);
    //近身攻击00
    NodeSetRange(this, cxRange2fv(0, 5));
    NodeSetAttackRate(this, 0.5f);
    cxSpriteSetTextureURL(this, "bg1.png");
    
    CX_METHOD_SET(this->Node.FindTarget, AttackFindTarget);
    CX_METHOD_SET(this->Node.AttackTarget, AttackAttackTarget);
}
CX_OBJECT_FREE(Attack, Node)
{
    
}
CX_OBJECT_TERM(Attack, Node)

Attack AttackCreate(cxAny map, cxSize2f size,cxVec2f pos)
{
    Attack this = CX_CREATE(Attack);
    NodeInit(this, map, size, pos);
    NodeSearchRun(this);
    return this;
}