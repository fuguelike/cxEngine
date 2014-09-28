//
//  LongAttacker.c
//  cxCore
//
//  Created by xuhua on 9/27/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <Bullet.h>
#include <actions/cxFollow.h>
#include <Map.h>
#include "LongAttacker.h"

static void BulletAttackArrive(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    CX_ASSERT_VALUE(cxActionView(pav), Bullet, bullet);
    //如果有bind的目标就攻击他
    cxHash bindes = cxViewBindes(bullet);
    CX_HASH_FOREACH(bindes, ele, tmp){
        Node target = cxHashElementKeyToAny(ele);
        //攻击目标
        NodeAttackTarget(bullet, target, AttackTypeBullet);
    }
    MapRemoveBullet(bullet);
}

static void LongAttackerTarget(cxAny pview,cxAny target,cxAny bd)
{
    CX_ASSERT_THIS(pview, LongAttacker);
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

CX_OBJECT_TYPE(LongAttacker, Attack)
{
    
}
CX_OBJECT_INIT(LongAttacker, Attack)
{
    NodeSetRange(this, cxRange2fv(0, 5));
    NodeSetAttackRate(this, 0.5f);
    CX_METHOD_SET(CX_TYPE(Node, this)->AttackTarget, LongAttackerTarget);
    
    cxViewSetColor(this, cxORANGE);
}
CX_OBJECT_FREE(LongAttacker, Attack)
{

}
CX_OBJECT_TERM(LongAttacker, Attack)

void LongAttackerInit(cxAny pview, cxAny pmap,cxVec2i pos)
{
    CX_ASSERT_THIS(pview, LongAttacker);
    AttackInit(this, pmap, pos);
}

LongAttacker LongAttackerCreate(cxAny pmap,cxVec2i pos)
{
    LongAttacker this = CX_CREATE(LongAttacker);
    LongAttackerInit(this, pmap, pos);
    return this;
}