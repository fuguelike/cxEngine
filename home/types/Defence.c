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
static void BulletAttackArrive(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    CX_ASSERT_VALUE(cxActionView(this), Bullet, bullet);
    //如果有bind的目标就攻击他
    cxHash bindes = cxViewBindes(bullet);
    CX_HASH_FOREACH(bindes, ele, tmp){
        Node target = cxHashElementKeyToAny(ele);
        //攻击目标
        NodeAttackTarget(bullet, target, AttackTypeBullet);
    }
    MapRemoveBullet(bullet);
}

//启动攻击
static void DefenceAttackTarget(cxAny pview,cxAny target,cxAny bd)
{
    CX_ASSERT_THIS(pview, Defence);
    Map map = NodeGetMap(this);
    //开火
    Bullet bullet = CX_CREATE(Bullet);
    BulletInit(bullet, map, cxSize2fv(20, 20), cxViewPosition(this));
    BulletSetPower(bullet, NodeGetPower(this));
    MapAppendBullet(bullet);
    //设定目标
    cxFollow f = cxFollowCreate(800, target);
    CX_EVENT_APPEND(f->cxAction.onExit, BulletAttackArrive);
    cxViewAppendAction(bullet, f);
    //子弹bind目标
    cxViewBind(bullet, target, cxNumberInt(NodeBindReasonShoot));
}

cxAny DefencePathRule(cxAny pview,cxAny target)
{
    CX_ASSERT_THIS(pview, Defence);
    //目标未达到攻击范围
    if(!NodeIsArriveRange(this, target)){
        return NULL;
    }
    return target;
}

cxAny DefenceFindRule(cxAny pview,const NodeCombined *type)
{
    CX_ASSERT_THIS(pview, Node);
    cxRange2f range = NodeGetRange(this);
    //搜索攻击范围内的目标
    return MapNearestQuery(this, *type, range);
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
    
    CX_METHOD_SET(this->Node.PathRule, DefencePathRule);
    CX_METHOD_SET(this->Node.AttackTarget, DefenceAttackTarget);
    CX_METHOD_SET(this->Node.FindRule, DefenceFindRule);
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

