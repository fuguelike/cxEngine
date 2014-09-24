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

static void DefenceAttackExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    //target 被 sp击中
    Node target = CX_TYPE_CAST(Node, this->target);
    Bullet sp = CX_TYPE_CAST(Bullet, this->cxAction.view);
    //模拟攻击
    NodeAddLife(target, -sp->power);
    CX_LOGGER("soldier life = %d",target->life);

    //目标死亡
    if(NodeIsDie(target)){
        cxViewUnBindAll(target);
        NodeRemove(target);
    }
    //隐藏子弹
    cxViewSetVisible(sp, false);
    cxViewRemove(sp);
}

void DefenceAttack(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    Map map = NodeMap(this);
    cxHash bindes = cxViewBindes(this);
    FixArray items = {0};
    CX_HASH_FOREACH(bindes, ele, tmp){
        //获取bind的目标
        Node target = cxHashElementKeyToAny(ele);
        //如果目标在范围外或者死亡
        if(!NodeAtRange(this, target) || NodeIsDie(target)){
            FixArrayAppend(items, target);
            continue;
        }
        
        //制造子弹 bullet
        Bullet sp = CX_CREATE(Bullet);
        BulletInit(sp, map, cxSize2fv(20, 20), cxViewPosition(this));
        BulletSetPower(sp, this->power);
        cxViewAppend(map->bullet, sp);
        //设定目标
        cxFollow f = cxFollowCreate(800, target);
        CX_EVENT_APPEND(f->cxAction.onExit, DefenceAttackExit);
        cxViewAppendAction(sp, f);
    }
    //移除范围外的
    for(cxInt i=0; i < items.number; i++){
        Node node = CX_TYPE_CAST(Node, items.items[i]);
        cxViewUnBind(this, node);
    }
}

void DefenceSearch(cxAny pview)
{
    CX_ASSERT_THIS(pview, Defence);
    Map map = NodeMap(this);
    cxHash bindes = cxViewBindes(this);
    //如果同时攻击数量到达
    if(cxHashLength(bindes) >= this->attackNum){
        return;
    }
    //搜索新目标
    Node node = NodeNearest(map->attacks, this->Node.idx, this->Node.range , NodeTypeAttack, NodeSubTypeNone);
    if(node == NULL){
        return;
    }
    //bind范围内的目标
    if(!NodeAtRange(this, node)){
        return;
    }
    cxViewBind(this, node, NULL);
}

CX_OBJECT_TYPE(Defence, Node)
{
    
}
CX_OBJECT_INIT(Defence, Node)
{
    this->Node.type = NodeTypeDefence;
    
    this->attackNum = 1;
    NodeSetAttackRate(this, 0.5f);
    NodeSetRange(this, cxRange2fv(2, 15));
    cxSpriteSetTextureURL(this, "bg1.png");
    
    CX_METHOD_SET(this->Node.Remove, MapRemoveDefence);
    CX_METHOD_SET(this->Node.Append, MapAppendDefence);
    
    CX_METHOD_SET(this->Node.Search, DefenceSearch);
    CX_METHOD_SET(this->Node.Attack, DefenceAttack);
    
    Range range = CX_CREATE(Range);
    RangeSetRange(range, this->Node.range);
    cxViewAppend(this, range);
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

