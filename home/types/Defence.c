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

static void DefenceSetAngleIndex(Defence this,Node target)
{
    cxInt index;
    cxFloat angle = cxVec2fRadiansBetween(cxViewPosition(target),cxViewPosition(this));
    AngleToIndex(angle, &index);
    if(this->index != index){
        this->index = index;
        CX_EVENT_FIRE(this, onIndex);
    }
    //Test
    cxConstChars n = CX_CONST_STRING("%d.png",this->index);
    cxSpriteSetTextureURL(this, n);
}

static void DefenceAttackExit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxFollow);
    //target 被 sp击中
    Node target = CX_TYPE_CAST(Node, this->target);
    Bullet sp = CX_TYPE_CAST(Bullet, this->cxAction.view);
    
    //模拟攻击
    NodeAddLife(target, -sp->power);
    CX_LOGGER("soldier life = %d",target->life);

    //隐藏子弹
    cxViewSetVisible(sp, false);
    cxViewRemove(sp);
}

void DefenceAttack(cxAny pview)
{
    CX_ASSERT_THIS(pview, Defence);
    Map map = NodeMap(this);
    cxHash bindes = cxViewBindes(this);
    CX_HASH_FOREACH(bindes, ele, tmp){
        //获取bind的目标
        Node target = cxHashElementKeyToAny(ele);
        DefenceSetAngleIndex(this,target);
        //如果目标在范围外或者死亡
        if(!NodeArriveAttack(this, target)){
            cxViewUnBind(this, target);
            continue;
        }
        //制造子弹 bullet
        Bullet sp = CX_CREATE(Bullet);
        BulletInit(sp, map, cxSize2fv(20, 20), cxViewPosition(this));
        BulletSetPower(sp, this->Node.power);
        cxViewAppend(map->bullet, sp);
        //设定目标
        cxFollow f = cxFollowCreate(800, target);
        CX_EVENT_APPEND(f->cxAction.onExit, DefenceAttackExit);
        cxViewAppendAction(sp, f);
    }
}

void DefenceSearch(cxAny pview)
{
    CX_ASSERT_THIS(pview, Defence);
    cxHash bindes = cxViewBindes(this);
    //如果同时攻击数量到达
    if(cxHashLength(bindes) >= this->attackNum){
        return;
    }
    //搜索最近的目标
    Node target = MapNearestQuery(this, this->Node.range, NodeTypeAttack, NodeSubTypeNone);
    if(target == NULL){
        return;
    }
    DefenceSetAngleIndex(this,target);
    if(!CX_METHOD_GET(true, this->Node.Finded,this,target)) {
        return;
    }
    if(!NodeArriveAttack(this, target)){
        return;
    }
    cxViewBind(this, target, NULL);
}

CX_OBJECT_TYPE(Defence, Node)
{
    
}
CX_OBJECT_INIT(Defence, Node)
{
    this->index = INT32_MAX;
    this->Node.type = NodeTypeDefence;
    this->Node.body = 1.5f;
    this->attackNum = 1;
    NodeSetAttackRate(this, 0.5f);
    NodeSetRange(this, cxRange2fv(0, 15));
    
    cxSpriteSetTextureURL(this, "bg1.png");
    CX_METHOD_SET(this->Node.Search, DefenceSearch);
    CX_METHOD_SET(this->Node.Attack, DefenceAttack);
    
    Range range = CX_CREATE(Range);
    RangeSetRange(range, this->Node.range);
    cxViewAppend(this, range);
}
CX_OBJECT_FREE(Defence, Node)
{
    CX_EVENT_RELEASE(this->onAttack);
    CX_EVENT_RELEASE(this->onIndex);
}
CX_OBJECT_TERM(Defence, Node)

Defence DefenceCreate(cxAny map,cxSize2f size,cxVec2f pos)
{
    Defence this = CX_CREATE(Defence);
    NodeInit(this, map, size, pos);
    NodeSearchRun(this);
    return this;
}

