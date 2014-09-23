//
//  Turret.c
//  Home
//
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxFollow.h>
#include <Map.h>
#include "Turret.h"
#include <Range.h>

static void TurretAttackExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    //target 被 sp击中
    Node target = CX_TYPE_CAST(Node, this->target);
    Map map = NodeMap(target);
    //获取攻击者
    cxHash binded = cxViewBinded(target);
    cxLong tag = cxActionTag(pav);
    Node firer = NULL;
    if(cxHashHas(binded, cxHashLongKey(tag))){
        firer = (Node)tag;
    }
    cxSprite sp = CX_TYPE_CAST(cxSprite, this->cxAction.view);
    //模拟攻击
    NodeAddLife(target, -20);
    CX_LOGGER("soldier life = %d",target->life);
    if(NodeIsDie(target)){
        //死去的攻击者移除
        MapRemoveAttack(map, target);
    }
    //隐藏子弹
    cxViewSetVisible(sp, false);
    cxViewRemove(sp);
}

static void TurretAttack(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    cxHash bindes = cxViewBindes(this);
    FixArray items = {0};
    CX_HASH_FOREACH(bindes, ele, tmp){
        //获取bind的目标
        Node target = cxHashElementKeyToAny(ele);
        //如果目标在范围外
        if(!NodeAtRange(this, target)){
            FixArrayAppend(items, target);
            continue;
        }
        
        //制造子弹 bullet
        cxSprite sp = cxSpriteCreateWithURL("shell.png");
        cxViewSetSize(sp, cxSize2fv(20, 20));
        cxViewSetPos(sp, this->cxSprite.cxView.position);
        cxViewAppend(this->map, sp);

        //
        cxFollow f = cxFollowCreate(800, target);
        //设置开火者
        cxActionSetTag(f, (cxLong)this);
        CX_EVENT_APPEND(f->cxAction.onExit, TurretAttackExit);
        cxViewAppendAction(sp, f);
    }
    //移除范围外的
    for(cxInt i=0; i < items.number; i++){
        Node node = CX_TYPE_CAST(Node, items.items[i]);
        cxViewUnBind(this, node);
        //node被lock解锁
        CX_METHOD_RUN(node->UnLock, node, this);
    }
}

static void TurretSearch(cxAny pview)
{
    CX_ASSERT_THIS(pview, Turret);
    Map map = NodeMap(this);
    cxHash bindes = cxViewBindes(this);
    //如果同时攻击数量到达
    if(cxHashLength(bindes) >= this->attackNum){
        return;
    }
    //搜索新目标
    NodeNearestInfo info = NodeNearest(map->attacks, this->Node.idx, this->Node.range , NodeTypeAttack, NodeSubTypeNone);
    if(info.node == NULL){
        return;
    }
    //bind范围内的目标
    Node node = info.node;
    if(!NodeAtRange(this, node)){
        return;
    }
    cxViewBind(this, node, NULL);
    //node被this锁定
    CX_METHOD_RUN(node->OnLock,node,this);
}

CX_OBJECT_TYPE(Turret, Node)
{
    
}
CX_OBJECT_INIT(Turret, Node)
{
    this->attackNum = 3;
    NodeSetAttackRate(this, 0.3f);
    NodeSetRange(this, cxRange2fv(5, 14));
    cxSpriteSetTextureURL(this, "bg1.png");
    
    CX_METHOD_SET(this->Node.Search, TurretSearch);
    CX_METHOD_SET(this->Node.Attack, TurretAttack);
    
    Range range = CX_CREATE(Range);
    RangeSetRange(range, this->Node.range);
    cxViewAppend(this, range);
}
CX_OBJECT_FREE(Turret, Node)
{

}
CX_OBJECT_TERM(Turret, Node)

Turret TurretCreate(cxAny map,cxSize2f size,cxVec2f pos)
{
    Turret this = CX_CREATE(Turret);
    NodeInit(this, map, size, pos, NodeTypeDefence, NodeSubTypeTurret);
    return this;
}

