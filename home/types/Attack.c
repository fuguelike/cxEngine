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

static void AttackAttack(cxAny pview,cxAny target)
{
    CX_ASSERT_THIS(pview, Attack);
    NodeAttackTarget(this, target, AttackTypeDirect);
}

static cxAny AttackFinded(cxAny pview,cxAny target,cxBool *isAttack)
{
    CX_ASSERT_THIS(pview, Attack);
    Map map = NodeMap(this);
    PathResult ret = MapSearchPath(this, target);
    if(ret.success){
        NodeMoveTo(this, MapSearchPoints(map));
        return target;
    }
    if(ret.block == NULL){
        return NULL;
    }
    ret = MapSearchPath(this, ret.block);
    if(ret.success){
        NodeMoveTo(this, MapSearchPoints(map));
        return ret.block;
    }
    return NULL;
}

CX_OBJECT_TYPE(Attack, Node)
{
    
}
CX_OBJECT_INIT(Attack, Node)
{
    NodeSetType(this, NodeTypeAttack);
    NodeSearchOrderAdd(this, NodeTypeDefence, NodeSubTypeNone);
    NodeSetBody(this, 0.3f);
    //近身攻击00
    NodeSetAttackRange(this, cxRange2fv(0, 0));
    NodeSetSearchRange(this, cxRange2fv(0, 60));
    NodeSetAttackRate(this, 0.5f);
    cxSpriteSetTextureURL(this, "bg1.png");
    
    CX_METHOD_SET(this->Node.Finded, AttackFinded);
    CX_METHOD_SET(this->Node.Attack, AttackAttack);
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