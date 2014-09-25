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

//
static void DefenceAttackExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    Bullet bullet = CX_TYPE_CAST(Bullet, this->cxAction.view);
    //如果有bind的目标就攻击他
    cxHash bindes = cxViewBindes(bullet);
    CX_HASH_FOREACH(bindes, ele, tmp){
        Node target = cxHashElementKeyToAny(ele);
        //攻击目标
        NodeAttackTarget(bullet, target, AttackTypeBullet);
    }
    cxViewRemove(bullet);
}

static void DefenceNodeAttacked(cxAny pview,cxAny attacker,AttackType type)
{
    NodeAttacked(pview, attacker, type);
}

static void DefenceAttackTarget(cxAny pview,cxAny target)
{
    CX_ASSERT_THIS(pview, Defence);
    Map map = NodeMap(this);
    Bullet bullet = CX_CREATE(Bullet);
    BulletInit(bullet, map, cxSize2fv(20, 20), cxViewPosition(this));
    BulletSetPower(bullet, NodePower(this));
    cxViewAppend(map->bullet, bullet);
    //设定目标
    cxFollow f = cxFollowCreate(800, target);
    CX_EVENT_APPEND(f->cxAction.onExit, DefenceAttackExit);
    cxViewAppendAction(bullet, f);
    //子弹bind目标
    cxViewBind(bullet, target, NULL);
}

static cxAny DefenceFindTarget(cxAny pview,cxAny target,cxBool *isAttack)
{
    CX_ASSERT_THIS(pview, Defence);
    //未达到攻击范围
    if(!NodeArriveAttack(this, target)){
        return NULL;
    }
    *isAttack = true;
    return target;
}

CX_OBJECT_TYPE(Defence, Node)
{
    
}
CX_OBJECT_INIT(Defence, Node)
{
    NodeSetType(this, NodeTypeDefence);
    NodeSetBody(this, 1.5f);
    NodeSetAttackRate(this, 0.5f);
    NodeSetRange(this, cxRange2fv(0, 15));
    
    NodeSetSearchOrder(this, NodeTypeAttack, NodeSubTypeNone,cxRange2fv(0, 15));
    
    cxSpriteSetTextureURL(this, "bg1.png");
    
    Range range = CX_CREATE(Range);
    RangeSetRange(range, NodeRange(this));
    cxViewAppend(this, range);
    
    CX_METHOD_SET(this->Node.FindTarget,   DefenceFindTarget);
    CX_METHOD_SET(this->Node.AttackTarget, DefenceAttackTarget);
    CX_METHOD_SET(this->Node.NodeAttacked, DefenceNodeAttacked);
}
CX_OBJECT_FREE(Defence, Node)
{
    
}
CX_OBJECT_TERM(Defence, Node)

Defence DefenceCreate(cxAny map,cxSize2f size,cxVec2f pos)
{
    Defence this = CX_CREATE(Defence);
    NodeInit(this, map, size, pos);
    NodeSearchRun(this);
    return this;
}

